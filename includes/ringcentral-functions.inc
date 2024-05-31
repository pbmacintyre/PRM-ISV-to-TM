<?php
/**
 * Copyright (C) 2024 Paladin Business Solutions
 *
 */
/* ================== */
/* Get RingCental SDK */
/* ================== */
function ringcentral_sdk () {
    // Include Libraries
    require('vendor/autoload.php');

    // get control data
    global $wpdb;

//    $result_rc = $wpdb->get_row($wpdb->prepare("SELECT * FROM `ringcentral_control`
//        WHERE `ringcentral_control_id` = %d", 1)
//    );

//    $client_id      = $result_rc->client_id ;
//    $client_secret  = $result_rc->client_secret ;
//    $jwt            = $result_rc->jwt_key ;

    $server = 'https://platform.ringcentral.com';
    $client_id = "4Wlqrn8Zjw9bZrvUfQqwFQ";
    $client_secret = "Ag2tTShoKr1e8S4bnGY7gt9q1WDGTdeTbfnYc52eRJhI";
    $jwt = "eyJraWQiOiI4NzYyZjU5OGQwNTk0NGRiODZiZjVjYTk3ODA0NzYwOCIsInR5cCI6IkpXVCIsImFsZyI6IlJTMjU2In0.eyJhdWQiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbS9yZXN0YXBpL29hdXRoL3Rva2VuIiwic3ViIjoiNjIxOTk0ODYwMTYiLCJpc3MiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbSIsImV4cCI6Mzg2NDU4NTQ3NSwiaWF0IjoxNzE3MTAxODI4LCJqdGkiOiJrYzlSTUluclJWbUZUYzUzLV9zVE1nIn0.RM2UOCBFfoLOzviuGGu47RLBxRohnFqeEO3IUwOhUEBHtJstf4n6VioeNSznidC58qoKcMb9BxoyxgWkVSz8Py-ucisxdVksDPdQdVbjSr-bvmJUox4iXNrSfzbBCilTjFB073kKhFuopQbeL2IG6dkp9-EuQ9nUhSddqj2Zq9yPxTJ2O_kbk8DbK0jMaCgzoNyA3l5E2imk8uZrfr324yIGXf8Lp1D0eopS5M_c2QrORXMbOU06IhY4aumXQpgU-LLkV3ogKretQ6UJHkhvhJKvW_ZmqGSBOcBhmTBINejS7vzhkulFA47QOmp3IYmLKwU8GAGg33rR3xssI7RwAw";

    $sdk = new RingCentral\SDK\SDK($client_id, $client_secret, $server);
    $platform = $sdk->platform();
    // Login via API
    if (!$platform->loggedIn()) {
        try {
            $platform->login(["jwt" => $jwt]);
        }
        catch (\RingCentral\SDK\Http\ApiException $e) {
            $platform = $e->getMessage();
        }
    }
    return $platform;
}

/* ============================================================ */
/* Validate RingCental SDK connection with provided credentials */
/* ============================================================ */
function ringcentral_validate_sdk ($rc_handshake) {
    // Include Libraries
    require('vendor/autoload.php');

    $server = 'https://platform.ringcentral.com';

    $sdk = new RingCentral\SDK\SDK($rc_handshake['client_id'],
        $rc_handshake['client_secret'], $server);

    // attempt to Login via API
    try {
        $sdk->platform()->login(["jwt" => $rc_handshake['jwt_key']]);
        $result = true;
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        $result = false;
        //echo "<br/><br/>Error trace: <br/>" . $e ;
    }

    return $result;
}

/* ============================================= */
/* Get list of Team Messaging Group Chats (Teams) */
/* ============================================= */
function list_tm_teams () {

    $rcsdk = ringcentral_sdk();

    $queryParams = array(
        'type' => array('Team'),
        'recordCount' => 250,
        //'type' => array( 'Everyone', 'Group', 'Personal', 'Direct', 'Team' ),
        //'recordCount' => 2,
    );
    try {
        $response = $rcsdk->get("/team-messaging/v1/chats", $queryParams);
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        $response = false;
        echo_spaces("error obj message", $e->getMessage());
    }

    return $response;
}

/* ============================== */
/* Build help icon and title text */
/* ============================== */
function ringcentral_build_help ($field) {
    global $wpdb;
    $image_source = RINGCENTRAL_PLUGINURL . 'images/question_mark.png';

    $result_rc_help = $wpdb->get_row($wpdb->prepare("SELECT ringcentral_help_help AS help_text
            FROM `ringcentral_help` WHERE `ringcentral_help_field` = %s", $field));

    $out_string = "<img src='$image_source' title='" . esc_attr($result_rc_help->help_text) . "' />";
    return $out_string;
}

/* ============================================ */
/* Get chat name based on provided chat number */
/* ============================================ */
function get_chat_name ($chatId) {
    $rcsdk = ringcentral_sdk();
    try {
        $response = $rcsdk->get("/team-messaging/v1/chats/{$chatId}");
        $chat_name = $response->json()->name;
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        $chat_name = false;
    }
    return $chat_name;
}

/* =============================================================== */
/* Triggered function when access form is submitted on public side */
/* =============================================================== */
function ringcentral_new_access_request ($fields) {
    global $wpdb;

    $message = "================================================= \r\n";
    $message .= "First Name: " . $fields[0]['first'] . "\r\n";
    $message .= "Last Name: " . $fields[0]['last'] . "\r\n";
    $message .= "email: " . $fields[1]['value'] . "\r\n";
    $message .= "Organization: " . $fields[3]['value'] . "\r\n";
    $message .= "Job Function: " . $fields[4]['value'] . "\r\n";
    $message .= "Have you published an app?: " . $fields[5]['value'] . "\r\n";
    $message .= "App listing URL: " . $fields[8]['value'] . "\r\n";
    $message .= "================================================= \r\n \r\n";

    // Get chat id from control table.
    $result_rc = $wpdb->get_row($wpdb->prepare("SELECT `chat_id` FROM `ringcentral_control` WHERE `ringcentral_control_id` = %d", 1));
    $chatId = $result_rc->chat_id;

    // post the message to the chat
    post_to_team_messaging($chatId, $message);
}

/* ============================================== */
/* Post text to Team Messaging Group Chats (Teams) */
/* ============================================== */
function post_to_team_messaging ($chatId, $message) {
    $rcsdk = ringcentral_sdk();

    $body = array(
        'text' => $message,
        // use options below if you are ever going to send attachments to the chat as well.
        // 'attachments' => array(
        //	array(
        //		'id' => '<ENTER VALUE>',
        //		'type' => '<ENTER VALUE>'
        //	)
    );

    try {
        $rcsdk->post("/team-messaging/v1/chats/{$chatId}/posts", $body);
        $result = true;
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        // echo "<br/><br/>Failed to add text to selected Team chat -><br/>" . $e ;
        $result = false;
    }
    return $result;
}

function echo_spaces ($text, $value = "", $lines = 0) {
    echo "<br /><strong><p style='color: red; display: inline'>$text:</p></strong> ";
    if (is_string($value) || is_int($value)) {
        echo $value;
    }
    if (is_object($value) || is_array($value)) {
        ringcentral_dump_obj($value);
    }
    if (is_bool($value)) {
        if ($value) echo "TRUE / Success"; else echo 'FALSE / Failure';
    }
    $i = 1;
    while ($i <= $lines) {
        echo "<br />";
        $i++;
    }
}

function show_errors () {
    error_reporting();
//     error_reporting(E_ALL & ~E_NOTICE & ~E_WARNING);
    ini_set('display_errors', 1);
}

function ringcentral_dump_obj ($object) {
    echo "<pre>";
    var_dump($object);
    echo "</pre>";
}

