<?php

/**
 * @file vkxp.pages.inc
 * Contains page and form for captcha processing.
 */

/**
 * Provides special form for captcha.
 */
function vkxp_captcha_page() {
  if (isset($_REQUEST['image']) && $_REQUEST['image']) {
    $form = drupal_get_form('vkxp_captcha_form', $_REQUEST['image']);
    return render($form);
  }
  else {
    drupal_access_denied();
  }
}

/**
 * Provides special form for captcha.
 */
function vkxp_captcha_form($form, &$form_state) {

  $image = $form_state['input']['image'];

  $form['image'] = array(
    '#type' => 'item',
    '#markup' => '<img src = "' . $image . '" />',
  );

  $form['text'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Enter captcha text'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send data'),
  );

  return $form;
}
