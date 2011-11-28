(function($) {

Drupal.behaviors.vkxp = function (context) {
  if (Drupal && Drupal.settings && Drupal.settings.vkxp) {

    //VK open api methods
    window.vkAsyncInit = function() {
      VK.init({
        apiId: Drupal.settings.vkxp.app_id
      });
      VK.Api.call(
        'wall.post',
        {
          owner_id: Drupal.settings.vkxp.owner_id,
          message: Drupal.settings.vkxp.message,
          from_group: Drupal.settings.vkxp.from_group,
          attachments: Drupal.settings.vkxp.attachments
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
