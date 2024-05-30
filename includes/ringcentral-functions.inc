<?php
/**
 * Copyright (C) 2024 Paladin Business Solutions
 *
 */
function echo_spaces ($text, $value = "") {
    $count = 50;
    for ($i = 1; $i <= $count; $i++) echo "&nbsp;";
    echo "<br /><strong><p style='color: red; display: inline'>$text:</p></strong> ";
    if (is_string($value) || is_int($value)) {
        echo $value;
    }
    if (is_object($value) || is_array($value)) {
        ringcentral_dump_obj($value);
    }
    echo "<br /><br />";
}

/* ================== */
/* Get RingCental SDK */
/* ================== */
function ringcentral_sdk () {
    // Include Libraries
    require('vendor/autoload.php');

    // get control data
    global $wpdb;

    $result_rc = $wpdb->get_row($wpdb->prepare("SELECT * FROM `ringcentral_control`
        WHERE `ringcentral_control_id` = %d", 1)
    );
//    $client_id      = $result_rc->client_id ;
//    $client_secret  = $result_rc->client_secret ;
//    $jwt            = $result_rc->jwt_key ;
    $jwt = "eyJraWQiOiI4NzYyZjU5OGQwNTk0NGRiODZiZjVjYTk3ODA0NzYwOCIsInR5cCI6IkpXVCIsImFsZyI6IlJTMjU2In0.eyJhdWQiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbS9yZXN0YXBpL29hdXRoL3Rva2VuIiwic3ViIjoiNjIxOTk0ODYwMTYiLCJpc3MiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbSIsImV4cCI6Mzg2NDU3ODY2NiwiaWF0IjoxNzE3MDk1MDE5LCJqdGkiOiJTeVc5QVQ0Y1JFMmFITVZVb01FY2lRIn0.amGDSRRQfXC4-osh20vaMIJJfQz7sgK5QNrnO3vuh7QopL-mrOXVDwzWwdXzMIJPHp_X_Sud2a6lXjU2oUrnZ5YY_Rb6wxuaYFRYX5p-L0GeGi3HLp7ZKx4iqAz4kCx9_Qix_H6aqTh1QdCFtvrJGNKsCM-LyrT-stgCfRcjpoE3j7zZvKxiOnLls71ZUlUH86izHqfYrcAnXJ7v6Grk2KJuFMDzDbvHXQgJ7y1i9QQtO6bZGjo-ofs4msMwIFqbVArIEuFIe0WUO7IvWVNkSveuxobIuwPGF94YpOPV00eCyjR06s_zHXTHDrjTfqIAslP0v80f9u6aDr12ScubfA";

    $server = 'https://platform.ringcentral.com';
    $client_id = "4Wlqrn8Zjw9bZrvUfQqwFQ";
    $client_secret = "Ag2tTShoKr1e8S4bnGY7gt9q1WDGTdeTbfnYc52eRJhI";

    $sdk = new RingCentral\SDK\SDK($client_id, $client_secret, $server);
    $platform = $sdk->platform();
    // Login via API
    if (!$platform->loggedIn()) {
        try {
            $platform->login(["jwt" => $jwt]);
            echo_spaces("Logged in to SDK?");
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
        $$sdk->platform()->login(["jwt" => $rc_handshake['jwt_key']]);
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
        //'type' => array( 'Everyone', 'Group', 'Personal', 'Direct', 'Team' ),
        //'recordCount' => 2,
    );
    try {
        $response = $rcsdk->get("/restapi/v1.0/glip/chats", $queryParams);
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

/* ============================== */
/* Encrypt Password to Obfusicate */
/* ============================== */
function ringcentral_encrypt ($plaintext) {
    if (function_exists('openssl_cipher_iv_length')) {
        $key = $_SERVER['SERVER_NAME'] . $_SERVER['SERVER_ADDR'] . $_SERVER['SERVER_SOFTWARE'];
        $cipher = "AES-128-CBC";
        $ivlen = openssl_cipher_iv_length($cipher);
        $iv = substr($c, 0, $ivlen);
        return base64_encode(openssl_encrypt($plaintext, $cipher, $key));
    }
    return $plaintext;
}

/* ============================== */
/* Decrypt Password to Obfusicate */
/* ============================== */
function ringcentral_decrypt ($ciphertext) {
    if (function_exists('openssl_cipher_iv_length')) {
        $key = $_SERVER['SERVER_NAME'] . $_SERVER['SERVER_ADDR'] . $_SERVER['SERVER_SOFTWARE'];
        $cipher = "AES-128-CBC";
        return openssl_decrypt(base64_decode($ciphertext), $cipher, $key);
    } else {
        return $ciphertext;
    }
}

/* ============================================ */
/* Get chant name based on provided chat number */
/* ============================================ */
function get_chat_name ($chat_num) {
    $rcsdk = ringcentral_sdk();
    $platform = $rcsdk->platform();

    $queryParams = array(
        'type' => array('Team'),
        //'type' => array( 'Everyone', 'Group', 'Personal', 'Direct', 'Team' ),
        //'recordCount' => 2,
    );

    try {
        $response = $platform->get("/restapi/v1.0/glip/chats", $queryParams);
        foreach ($response->json()->records as $record) {
            if ($chat_num == $record->id) {
                $chat_name = $record->name;
            }
        }
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        $chat_name = false;
        // $response = $e ;
    }

    return $chat_name;
}

/* =============================================================== */
/* Triggered function when access form is submitted on public side */
/* =============================================================== */
function ringcentral_new_access_request ($fields) {
    global $wpdb;
//    echo "<pre>" ;
//    var_dump($fields) ;
//    echo "</pre>" ;

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
    $result_rc = $wpdb->get_row($wpdb->prepare("SELECT `ringcentral_team_chat` FROM `ringcentral_control` WHERE `ringcentral_control_id` = %d", 1));
    $chatId = $result_rc->ringcentral_team_chat;

    // post the message to the chat
    post_to_team_messaging($chatId, $message);
}

/* ============================================== */
/* Post text to Team Messaging Group Chats (Teams) */
/* ============================================== */
function post_to_team_messaging ($chatId, $message) {
    $rcsdk = ringcentral_sdk();
    $platform = $rcsdk->platform();

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
        $platform->post("/restapi/v1.0/glip/chats/{$chatId}/posts", $body);
        $result = true;
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        // echo "<br/><br/>Failed to add text to selected Team chat -><br/>" . $e ;
        $result = false;
    }
    return $result;
}

?>