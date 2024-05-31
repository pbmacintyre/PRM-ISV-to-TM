<?php

require('includes/vendor/autoload.php');
require('includes/ringcentral-functions.inc');

show_errors();

$rcsdk = ringcentral_sdk();

$queryParams = array(
    'type' => array( 'Team' ),
    'recordCount' => 250,
    //'pageToken' => '<ENTER VALUE>'
);
$i = 1;
try {
    $r = $rcsdk->get("/team-messaging/v1/chats", $queryParams);
    // echo_spaces("chat groups", $r->json());

    foreach ($r->json()->records as $record) {
        echo_spaces("Team ID / Name", "[" . $i++ . "] " .$record->id . " => " . $record->name ) ;
    }
} catch (Exception $e) {
    echo_spaces("Error MesSaGe", $e->getMessage());
}
