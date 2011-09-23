<?php

/**
 * Page callback
 * Return form with main settings
 */
function vkxp_admin_main_settings() {

  //process code request
  if ($_GET['code']){
    $params = array();
    $params['client_id'] = variable_get('vkxp_app_id', 0);
    $params['client_secret'] = variable_get('vkxp_app_secret', 0);
    $params['code'] = $_GET['code'];
    $data = http_build_query($params, '', '&');
    $ch = curl_init();
    curl_setopt ($ch, CURLOPT_URL, 'https://api.vkontakte.ru/oauth/access_token');
    curl_setopt ($ch, CURLOPT_POST, 1);
    curl_setopt ($ch, CURLOPT_POSTFIELDS, $data);
    curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
    $result = curl_exec($ch);
    curl_close($ch);
    $json_result = json_decode($result, true);
    if ($json_result['access_token']) {
      variable_set('vkxp_access_token', $json_result['access_token']);
    }
  }

  $form = array();

  $form['vkxp_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable VKontakte crossposter'),
    '#default_value' => variable_get('vkxp_enable', 0),
  );

  $form['vkxp_main'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main settings'),
  );

  $form['vkxp_main']['vkxp_group_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Group ID'),
    '#required' => true,
    '#default_value' => variable_get('vkxp_group_id', ''),
  );

  $form['vkxp_main']['vkxp_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#required' => true,
    '#default_value' => variable_get('vkxp_app_id', ''),
  );

  $form['vkxp_main']['vkxp_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Application secret code'),
    '#required' => true,
    '#default_value' => variable_get('vkxp_app_secret', ''),
  );

  $form['vkxp_main']['vkxp_official'] = array(
    '#type' => 'checkbox',
    '#title' => t('Write from group name'),
    '#default_value' => variable_get('vkxp_official', 1),
  );

  $form = system_settings_form($form);

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset cookies'),
  );

  $form['#submit'][] = 'vkxp_admin_settings_submit';

  return $form;
}

/**
 * Submit function for vkxp main settings
 */
function vkxp_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['vkxp_app_id']) {
    variable_set('vkxp_user_id', $values['vkxp_app_id']);
    $params = array();
    $params['client_id'] = trim($values['vkxp_app_id']);
    $params['scope'] = 'wall,groups,photos,offline';
    $params['display'] = 'page';
    $params['redirect_uri'] = url('admin/settings/vkxp/main', array('absolute' => TRUE));
    $params['response_type'] = 'code';
    $data = http_build_query($params, '', '&');
    $url = 'http://api.vkontakte.ru/oauth/authorize?'. $data;
    drupal_goto($url);
  }
}

/**
 * Page callback
 * Return form with node settings
 */
function vkxp_admin_node_settings() {
  $form = array();

  $form['node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
  );

  $options = node_get_types('names');
  $form['node_types']['vkxp_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types to crosspost'),
    '#options' => $options,
    '#default_value' => variable_get('vkxp_node_types', array()),
  );

  $form['vkxp_body'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messages'),
  );

  $form['vkxp_body']['vkxp_cut_body'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cut message'),
    '#default_value' => variable_get('vkxp_cut_body', 1),
  );

  $form['vkxp_body']['vkxp_cut_body_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Cut if message length more than'),
    '#field_suffix' => t('symbols'),
    '#size' => 4,
    '#default_value' => variable_get('vkxp_cut_body_length', 255),
  );

  return system_settings_form($form);
}

/**
 * Return form width vkxp image settings
 */
function vkxp_admin_images_settings() {
  $form = array();
  // Get node types that should be crossposted
  $selected_node_types = _vkxp_get_selected_node_types();
  if (!$selected_node_types) {
    return t('Please, select content types to see their image fiels');
  }

  if (module_exists('imagefield')) {
    $image_fields = array();
    $content_info = _content_type_info();
    foreach ($selected_node_types as $node_type) {

      // Build fieldset for every selected node type
      $node_types = node_get_types('names');
      $form['vkxp_node_type_'. $node_type] = array(
        '#type' => 'fieldset',
        '#title' => t($node_types[$node_type]),
      );

      // If node type has cck fields we should find imagefield
      if($content_info['content types'][$node_type]['fields']) {
        $fields = $content_info['content types'][$node_type]['fields'];
        foreach ($fields as $field) {
          if ($field['widget']['module'] == 'imagefield') {
            $image_fields[$node_type][$field['field_name']] = $field['field_name'];
          }
        }
      }

      // If imagefield for this node type found - build settings form for that
      if ($image_fields[$node_type]) {
        $form['vkxp_node_type_'. $node_type][$node_type .'_image_amount'] = array(
          '#type' => 'select',
          '#title' => t('Select amount of images'),
          '#options' => drupal_map_assoc(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
          '#default_value' => variable_get($node_type .'_image_amount', 0),
          '#description' => t('Amount of images which will be posted to vkontakte.ru'),
        );
        $form['vkxp_node_type_'. $node_type][$node_type .'_image_field'] = array(
          '#type' => 'select',
          '#title' => t('Select imagefield'),
          '#options' => $image_fields[$node_type],
          '#default_value' => variable_get($node_type .'_image_field', ''),
          '#description' => t('Selected field will be posted with message to vkontakte.ru'),
        );
      }
      else {
        $form['vkxp_node_type_'. $node_type]['empty_value'] = array('#value' => t('This node types does not contain image fields'));
      }
    }
  }
  else {
    return t("You can't post images until !imagefield module is installed.", array('!imagefield' => '<a href = "http://drupal.org/project/imagefield">imagefield</a>'));
  }

  $form = system_settings_form($form);
  unset($form['buttons']['reset']);
  return $form;
}