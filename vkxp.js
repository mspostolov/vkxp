(function($) {

Drupal.behaviors.vkxp = {
  attach: function (context, settings) {

    // Make sure this behavior is not processed more than once.
    if (this.processed) {
      return;
    }
    this.processed = true;

    $.getScript('http://vk.com/js/api/openapi.js', function() {

      // Initialize Open API.
      VK.init({
        apiId: settings.vkxp.app_id
      });

      // Post node to vk wall.
      VK.Api.call(
        'wall.post',
        {
          owner_id: settings.vkxp.owner_id,
          message: settings.vkxp.message,
          from_group: settings.vkxp.from_group,
          attachments: settings.vkxp.attachments
        },
        function(response) {

          // If captcha needed.
          if (response.error && response.error.error_code == 14) {
            vkxp_send_captcha(response, settings);
          }
        }
      );

    });
  }
};

function vkxp_send_captcha(response, settings) {

  $('body').prepend('<div id="vkxp-form"></div>');

  $('#vkxp-form')
    .load(
      settings.basePath + 'vkxp/captcha #vkxp-captcha-form',
      { image: response.error.captcha_img }
    )
    .dialog({
      title: Drupal.t('Enter vk captcha'),
      resizable: false,
      modal: true
    });

  $('#vkxp-captcha-form').live('submit', function() {
    $text = $(this).find('.form-text').val();

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

    $('#vkxp-form').dialog('destroy').remove();
    return false;
  });
}

})(jQuery);