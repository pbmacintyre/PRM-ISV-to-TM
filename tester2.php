<?php

require('includes/vendor/autoload.php');
require('includes/ringcentral-functions.inc');

show_errors();

$rcsdk = ringcentral_sdk();

$chatId = '136080719878' ;

try {
    $r = $rcsdk->get("/team-messaging/v1/chats/{$chatId}");
    echo_spaces("chat details", $r->json());
    echo_spaces("Team ID / Name", $r->json()->id . " => " . $r->json()->name ) ;

} catch (Exception $e) {
    echo_spaces("Error", $e->getMessage());
}

