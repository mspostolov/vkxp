***ENGLISH***

Install
=======

1) Go to http://vkontakte.ru/editapp?act=create&site=1 and create STANDALONE application (not web-site!).

2) Extract module in sites/all/modules directory

3) Go to module settings page /admin/build/modules and enable vkxp (VKontakte CrossPoster) module

4) Go to vkxp settings page /admin/settings/vkxp and paste there application ID, application secret key (from application settings page) and group ID (you should be administrator there).

5) Click 'Save configuration' button and give your application access to your web site

6) Don't forget to select node types (/admin/settings/vkxp/node) and images (/admin/settings/vkxp/images) that should be posted


Usage
=====

When creating or editing node, just check "Post this node to vkontakte.ru" and data will be automatically sent to vkontakte.ru


Theme
=====

If you are authorized on vk server (if you get access token after 5th step of installation) you may make queries to vk api using this function:

theme('vkxp_query', $api_method, $post_fields);

About VK api you can read here http://vkontakte.ru/developers.php#devstep2

Example (get wall upload server)
---------------------------------

$params = array();
$params['gid'] = variable_get('vkxp_group_id', '');
$params['access_token'] = variable_get('vkxp_access_token', '');
$result = theme('vkxp_query', 'photos.getWallUploadServer', $params);