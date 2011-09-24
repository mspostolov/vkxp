<?php

/**
 * @file
 * Theme function used for VK api queries 
 */

/**
 * Makes http query to VK api using cURL library
 * @param  $method: vkontakte api method (http://vkontakte.ru/developers.php#devstep2)
 * @param  $params: array of post fields
 * @param  $request_url: url to make query
 * @return server response
 */
function theme_vkxp_query($method, $params, $request_url = 'https://api.vkontakte.ru/method/') {
  $curl = curl_init();
  curl_setopt ($curl, CURLOPT_URL, $request_url . $method);
  curl_setopt ($curl, CURLOPT_POST, 1);
  curl_setopt ($curl, CURLOPT_POSTFIELDS, $params);
  curl_setopt ($curl, CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_setopt ($curl, CURLOPT_RETURNTRANSFER, 1);
  $result = curl_exec($curl);
  curl_close($curl);
  return json_decode($result, true);
}