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
 * Submit function for vkxp settings
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

  $form['vkxp_title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Titles'),
  );

  $form['vkxp_title']['vkxp_cut_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cut title'),
    '#default_value' => variable_get('vkxp_cut_title', 1),
  );

  $form['vkxp_title']['vkxp_cut_title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Cut if title length more than'),
    '#field_suffix' => t('symbols'),
    '#size' => 4,
    '#default_value' => variable_get('vkxp_cut_title_length', 81),
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