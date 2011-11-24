Install
=======

1) Go to http://vkontakte.ru/apps.php?act=add&site=1 and create web site application

2) Extract module in sites/all/modules directory

3) Go to module settings page /admin/modules and enable vkxp (VKontakte CrossPoster) module

4) Go to vkxp settings page /admin/config/services/vkxp and paste there application ID, application secret key (from application settings page) and group ID (you should be administrator there).

5) Click 'Save configuration' button and give your application access to your web site

6) Don't forget to select node types (/admin/config/services/vkxp/node) and images (/admin/config/services/vkxp/images) that should be posted


Usage
=====

When creating or editing node, just check "Post this node to vkontakte.ru" and data will be automatically sent to vkontakte.ru


External usage
==============

If you are authorized on vk server (if you get access token after 5th step of installation) you may make queries to vk api using this function:

vkxp_query($api_method, $post_fields, $requert_url);

About VK api you can read here http://vkontakte.ru/developers.php#devstep2

Also it is possible to change this query using hook_vkxp_query_alter(). Read about it in vkxp.api.php
