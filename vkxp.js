(function($) {

Drupal.behaviors.vkxp = {
  attach: function (context, settings) {

    //vk open api methods
    window.vkAsyncInit = function() {
      VK.init({
        apiId: settings.vkxp.app_id
      });
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
    };

    // Async script loading
    setTimeout(function() {
      var el = document.createElement("script");
      el.type = "text/javascript";
      el.src = "http://vkontakte.ru/js/api/openapi.js";
      el.async = true;
      document.getElementById("vk_api_transport").appendChild(el);
    }, 0);
    
  }
};

})(jQuery);
