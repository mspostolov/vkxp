<?php

/**
 * @file
 * Contains vkxp settings forms.
 */

/**
 * Page callback.
 * Return form with VKXP main settings.
 */
function vkxp_admin_main_settings() {

  // Process request from vk.
  if (isset($_GET['code'])) {
    $params = array();
    $params['client_id'] = trim(variable_get('vkxp_app_id', 0));
    $params['client_secret'] = trim(variable_get('vkxp_app_secret', 0));
    $params['code'] = $_GET['code'];
    $result = vkxp_query('', $params, 'https://api.vkontakte.ru/oauth/access_token');
    if (isset($result['access_token'])) {
      variable_set('vkxp_access_token', $result['access_token']);
      _vkxp_watchdog(
        array(
          'text' => t('Access token was recieved from vkontakte. Now you may post your nodes there.'),
          'severity' => 'status'
        )
      );
    }
    else {
      _vkxp_watchdog(
        array('text' => t('Access token was not recieved from vkontakte. Error: !error (!error_description)',
        array('!error' => $result['error'], '!error_description' => $result['error_description'])), 'severity' =>  'error')
      );
    }
  }
  elseif (isset($_GET['error'])) {
    _vkxp_watchdog(
      array('text' => t('Access code was not recieved from vkontakte. Error: !error (!error_description)',
      array('!error' => $_GET['error'], '!error_description'=> $_GET['error_description'])), 'severity' => 'error')
    );
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
    '#title' => t('Owner ID'),
    '#required' => true,
    '#default_value' => variable_get('vkxp_group_id', ''),
  );

  $form['vkxp_main']['vkxp_wall_owner'] = array(
    '#type' => 'select',
    '#title' => t('Select owner type'),
    '#options' => array(
      'group' => t('Group'),
      'user' => t('User'),
    ),
    '#default_value' => variable_get('vkxp_wall_owner', 'group'),
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

  $form['vkxp_main']['vkxp_enabled_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Checkbox "Post this node to vkontakte.ru" are checked by default'),
    '#default_value' => variable_get('vkxp_enabled_default', 0),
    '#description' => t('Check this if you want checkbox "Post this node to vkontakte.ru" in node form was checked by default')
  );

  $form['vkxp_main']['vkxp_official'] = array(
    '#type' => 'checkbox',
    '#title' => t('Write from group name'),
    '#default_value' => variable_get('vkxp_official', 1),
    '#description' => t('Check this if you want to post messages from group name'),
  );
  
  $form['vkxp_main']['vkxp_add_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add link on wall to posted page'),
    '#default_value' => variable_get('vkxp_add_link', 0),
    '#description' => t('Check this if you want to post node url on vk wall'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Recieve new access token'),
  );

  return $form;
}

/**
 * Submit callback for vkxp main settings form.
 */
function vkxp_admin_main_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Save values.
  variable_set('vkxp_enable'         , $values['vkxp_enable']);
  variable_set('vkxp_group_id'       , trim($values['vkxp_group_id']));
  variable_set('vkxp_wall_owner'     , $values['vkxp_wall_owner']);
  variable_set('vkxp_enabled_default', $values['vkxp_enabled_default']);
  variable_set('vkxp_official'       , $values['vkxp_official']);
  variable_set('vkxp_add_link'       , $values['vkxp_add_link']);

  // If application ID or application secret key was changed we should get new access token.
  $app_id_changed = variable_get('vkxp_app_id', 0) != $values['vkxp_app_id'];
  $secret_changed = variable_get('vkxp_app_secret', 0) != $values['vkxp_app_secret'];
  $reset_clicked  = $values['op'] == $form['reset']['#value'];
  if ($app_id_changed || $secret_changed || $reset_clicked) {
    variable_set('vkxp_app_id', trim($values['vkxp_app_id']));
    variable_set('vkxp_app_secret', trim($values['vkxp_app_secret']));
    $params = array();
    $params['client_id'] = trim($values['vkxp_app_id']);
    $params['scope'] = 'wall,groups,photos,offline';
    $params['display'] = 'page';
    $params['redirect_uri'] = url('admin/config/services/vkxp/main', array('absolute' => TRUE));
    $params['response_type'] = 'code';
    $data = http_build_query($params, '', '&');
    $url = 'http://api.vkontakte.ru/oauth/authorize?' . $data;
    drupal_goto($url);
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Page callback.
 * Return form with node settings.
 */
function vkxp_admin_node_settings() {

  $form = array();

  $form['node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
  );

  $form['node_types']['vkxp_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types to crosspost'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('vkxp_node_types', array()),
  );

  $form['vkxp_body'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messages'),
  );

  $form['vkxp_body']['vkxp_post_object'] = array(
    '#type' => 'select',
    '#title' => t('Message body'),
    '#description' => t('Select what to post as wall message'),
    '#options' => array(
      'title' => t('Node title'),
      'body' => t('Node body'),
      'title_body' => t('Node title and node body'),
    ),
    '#default_value' => variable_get('vkxp_post_object', 'body'),
  );

  $form['vkxp_body']['vkxp_cut_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cut message'),
    '#default_value' => variable_get('vkxp_cut_message', 1),
  );

  $form['vkxp_body']['vkxp_cut_message_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Cut if message length more than'),
    '#field_suffix' => t('symbols'),
    '#size' => 4,
    '#default_value' => variable_get('vkxp_cut_message_length', 255),
  );

  return system_settings_form($form);
}

/**
 * Return form width vkxp image settings.
 */
function vkxp_admin_images_settings() {
  $form = array();

  // Get node types that should be crossposted.
  $selected_node_types = _vkxp_get_selected_node_types();
  if (!$selected_node_types) {
    $form['image'] = array('#markup' => t('Please, select content types to see their image fiels'));
    return $form;
  }

  if (module_exists('image')) {
    $image_fields = array();
    $fields_info = field_info_fields();
    foreach ($selected_node_types as $node_type) {

      // Build fieldset for every selected node type.
      $node_types = node_type_get_names();
      $form['vkxp_node_type_' . $node_type] = array(
        '#type' => 'fieldset',
        '#title' => $node_types[$node_type],
      );

      foreach ($fields_info as $field) {
        if ($field['type'] == 'image' && isset($field['bundles']['node'])) {
          $bundles = $field['bundles']['node'];
          if (in_array($node_type, $bundles)) {
            $image_fields[$node_type][$field['field_name']] = $field['field_name'];
          }
        }
      }

      // If imagefield for this node type found - build settings form for it.
      if (isset($image_fields[$node_type])) {
        $form['vkxp_node_type_' . $node_type]['vkxp_' . $node_type . '_image_amount'] = array(
          '#type' => 'select',
          '#title' => t('Select amount of images'),
          '#options' => drupal_map_assoc(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
          '#default_value' => variable_get('vkxp_' . $node_type . '_image_amount', 0),
          '#description' => t('Amount of images which will be posted to vkontakte.ru'),
        );
        $form['vkxp_node_type_' . $node_type]['vkxp_' . $node_type . '_image_field'] = array(
          '#type' => 'select',
          '#title' => t('Select imagefield'),
          '#options' => $image_fields[$node_type],
          '#default_value' => variable_get('vkxp_' . $node_type . '_image_field', ''),
          '#description' => t('Selected field will be posted with message to vkontakte.ru'),
        );
      }
      else {
        $form['vkxp_node_type_' . $node_type]['empty_value'] = array('#markup' => t('This node types does not contain image fields'));
      }
    }
  }
  else {
    $form['image'] = array('#markup' => t("You can't post images until IMAGE module is installed."));
    return $form;
  }

  return system_settings_form($form);
}
