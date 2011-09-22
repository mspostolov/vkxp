<?php

/**
 * Page callback
 * Return form with main settings
 */
function vkxp_admin_main_settings() {

  if ($_GET['code']){
    //https://api.vkontakte.ru/oauth/access_token?client_id=APP_ID&client_secret=APP_SECRET&code=$code
    $params = array();
    $params['client_id'] = 2494902;
    $params['client_secret'] = 'P7wcna3mCRxcbvaeL4kq';
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
    '#default_value' => variable_get('vkxp_group_id', 0),
  );

  $form['vkxp_main']['vkxp_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#required' => true,
    '#default_value' => variable_get('vkxp_login', ''),
  );

  if (!variable_get('vkxp_hide_password', 0)) {
    $form['vkxp_main']['vkxp_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#default_value' => variable_get('vkxp_password', ''),
    );
    $form['vkxp_main']['vkxp_hide_password'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide password'),
      '#default_value' => variable_get('vkxp_hide_password', 0),
      '#description' => t("Check this option to permanently hide the plaintext password from peeking eyes. You may still change the password afterwards, but it won't be displayed anymore."),
    );
  }
  else {
    $have_password = (variable_get('vkxp_password', '') != '');
    $form['vkxp_main']['vkxp_password'] = array(
      '#type' => 'password',
      '#title' => $have_password ? t('Change password') : t('Password'),
      '#description' => $have_password ? t('Leave empty if you do not intend to change the current password.') : '',
    );
  }

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

  //http://api.vkontakte.ru/oauth/authorize?client_id=APP_ID&scope=SETTINGS&redirect_uri=REDIRECT_URI&response_type=code
  $params = array();
  $params['client_id'] = 2494902 ;
  $params['scope'] = 'wall,groups,photos,offline';
  $params['display'] = 'page';
  $params['redirect_uri'] = url('admin/settings/vkxp/main', array('absolute' => TRUE));
  $params['response_type'] = 'code';
  $data = http_build_query($params, '', '&');
  //$data = 'client_id=2494719&scope=wall,offline,groups&redirect_uri=http://localhost:81/duel/&response_type=code';
  $url = 'http://api.vkontakte.ru/oauth/authorize?'. $data;
  drupal_goto($url);
  /*$ch = curl_init();
  curl_setopt ($ch, CURLOPT_URL, 'http://api.vkontakte.ru/oauth/authorize');
  curl_setopt ($ch, CURLOPT_POST, 1);
  curl_setopt ($ch, CURLOPT_POSTFIELDS, $data);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
  $result = curl_exec($ch);
  curl_close($ch);*/


  /*$values = $form_state['values'];
  // If we have no cookies we should make file for it
  if (!variable_get('vkxp_cookies', '')) {
    if ($values['vkxp_group_id'] && $values['vkxp_login'] && $values['vkxp_official']) {
      // Generate unique cookie filename
      $hash = md5($values['vkxp_group_id'] . $values['vkxp_login'] . $values['vkxp_official']);
      variable_set('vkxp_cookies', $hash .'.txt');
    }
  }

  // Reset cookies
  // This may help if user did something wrong and now he can't post his data
  if ($values['op'] == $form['reset']['#value']) {
    $cookies = md5($values['vkxp_group_id'] . $values['vkxp_login'] . $values['vkxp_official']) . '.txt';
    variable_set('vkxp_cookies', $cookies);
    _vkxp_authorization($cookies);
    drupal_set_message(t('New cookie file was generated.'));
  }*/
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