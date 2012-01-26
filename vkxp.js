(function($) {

Drupal.behaviors.vkxp = {
  attach: function (context, settings) {

    $.getScript('http://vk.com/js/api/openapi.js', function() {

      // Initialize VK OPEN API
      VK.init({
        apiId: settings.vkxp.app_id
      });

      // Post node to vk wall
      VK.Api.call(
        'wall.post',
        {
          owner_id: settings.vkxp.owner_id,
          message: settings.vkxp.message,
          from_group: settings.vkxp.from_group,
          attachments: settings.vkxp.attachments
        },
        function(response) {
          // Here you may add some response callback
        }
      );
    });
    
  }
};

})(jQuery);
