(function($) {

Drupal.behaviors.vkxp = function (context) {
  if (Drupal && Drupal.settings && Drupal.settings.vkxp) {

    $.getScript('http://vk.com/js/api/openapi.js', function() {

      // Initialize VK OPEN API
      VK.init({
        apiId: Drupal.settings.vkxp.app_id
      });

      // Post node to vk wall
      VK.Api.call(
        'wall.post',
        {
          owner_id: Drupal.settings.vkxp.owner_id,
          message: Drupal.settings.vkxp.message,
          from_group: Drupal.settings.vkxp.from_group,
          attachments: Drupal.settings.vkxp.attachments
        },
        function(response) {
          // If captcha needed.
          console.log(response);
          if (response.error && response.error.error_code == 14) {
            vkxp_send_captcha(response, Drupal.settings);
          }
        }
      );

    });

  }
};

function vkxp_send_captcha(response, settings) {

  $('body').prepend('<div id="vkxp-form"></div>');

  $('#vkxp-form')
    .load(settings.basePath + 'vkxp/captcha #vkxp-captcha-form',
      {
        image: response.error.captcha_img
      }
    )
    .dialog({
      title: Drupal.t('Enter vk captcha'),
      resizable: false,
      modal: true
    });

  $('#vkxp-captcha-form').live('submit', function() {
    var $text = $(this).find('.form-text').val();

    if (response.error && response.error.captcha_sid) {
      VK.Api.call('wall.post',
        {
          owner_id: settings.vkxp.owner_id,
          message: settings.vkxp.message,
          from_group: settings.vkxp.from_group,
          attachments: settings.vkxp.attachments,
          captcha_sid: response.error.captcha_sid,
          captcha_key: $text
        },
        function (new_response) {
          // If captcha needed.
          if (new_response.error && new_response.error.error_code == 14) {
            vkxp_send_captcha(new_response, settings);
          }
        }
      );
    }

    $('#vkxp-form').dialog('destroy').remove();
    return false;
  });
}

})(jQuery);
