<?php
/**
 * Copyright (C) 2022 Paladin Business Solutions
 *
 */

/* ================== */
/* Get RingCental SDK */
/* ================== */
function ringcentral_sdk() {
    // Include Libraries
    require('vendor/autoload.php');
    
    // get control data
    global $wpdb;
    
    $result_rc = $wpdb->get_row( $wpdb->prepare("SELECT * FROM `ringcentral_control`
        WHERE `ringcentral_control_id` = %d", 1)
        );
    $client_id             = $result_rc->client_id ;
    $client_secret         = $result_rc->client_secret ;
    $ringcentral_user_name = $result_rc->ringcentral_user_name ;
    $ringcentral_extension = $result_rc->ringcentral_extension ;
    // $ringcentral_password  = $result_rc->ringcentral_password;
    $ringcentral_password  = ringcentral_decrypt($result_rc->ringcentral_password);
    $ringcentral_sandbox   = $result_rc->ringcentral_sandbox ;
    
    if ($ringcentral_sandbox == 1) {
        // Use sandbox Client
        $sdk = new RingCentral\SDK\SDK($client_id, $client_secret, 
            RingCentral\SDK\SDK::SERVER_SANDBOX);        
    } else {
        // Use Production Client
        $sdk = new RingCentral\SDK\SDK($client_id, $client_secret, 
            RingCentral\SDK\SDK::SERVER_PRODUCTION);
    }    
    
    // Login via API
    if (!$sdk->platform()->loggedIn()) {
        $sdk->platform()->login($ringcentral_user_name, 
            $ringcentral_extension, $ringcentral_password);
    }
    return $sdk ;
}
/* ============================================================ */
/* Validate RingCental SDK connection with provided credentials */
/* ============================================================ */
function ringcentral_validate_sdk($rc_handshake) {
    // Include Libraries
    require('vendor/autoload.php');
    
    if ($rc_handshake['sandbox'] == 1) {
        // Use sandbox Client        
        $sdk = new RingCentral\SDK\SDK($rc_handshake['client_id'], 
            $rc_handshake['client_secret'], RingCentral\SDK\SDK::SERVER_SANDBOX);        
    } else {
        // Use Production Client
        $sdk = new RingCentral\SDK\SDK($rc_handshake['client_id'], 
            $rc_handshake['client_secret'], RingCentral\SDK\SDK::SERVER_PRODUCTION);        
    }
    
    // attempt to Login via API
    try {
        $sdk->platform()->login($rc_handshake['user_name'], $rc_handshake['extension'], 
            $rc_handshake['password']);
        $result = true ;        
    } catch (\RingCentral\SDK\Http\ApiException $e) {
        $result = false ;
        //echo "<br/><br/>Error trace: <br/>" . $e ;
    }
	
	/*
	// Set webhook if possible
	if (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off' && $result) {
		
		$webhook_url = add_query_arg(array('rcwebhook'=>'true'), get_site_url(null, '', 'https'));
		
		$webhookInstalled = 0;
		$apiResponse = $sdk->platform()->get('/subscription');
		foreach ($apiResponse->json()->records as $record) {
			if ($record->deliveryMode->address == $webhook_url) {
				$webhookInstalled = 1;
			}
		}
		
		if (!$webhookInstalled) {
			$params = array(
				'eventFilters' => array(
					'/restapi/v1.0/account/~/extension/~/message-store/instant?type=SMS'
					),
				'deliveryMode' => array(
					'transportType' => 'WebHook',
					'address' => $webhook_url,
					'expiresIn' => '630720000',
				)
			);
			
			try {
				$sdk->platform()->post('/subscription', $params);
			} catch (\RingCentral\SDK\Http\ApiException $e) {
				// Unable to create webhook
				echo "<br/><br/>Error trace, Webhook: <br/>" . $e ;
			}
		}
	} */
    return $result ;
}
/* ============================================= */
/* Get list of Team Messaging Group Chats (Teams) */
/* ============================================= */
function list_tm_teams() {

    $rcsdk = ringcentral_sdk();

    $platform = $rcsdk->platform();
	
	$queryParams = array(
			'type' => array('Team'),
			//'type' => array( 'Everyone', 'Group', 'Personal', 'Direct', 'Team' ),
			//'recordCount' => 2,			
	);
	
	try {
		$response = $platform->get("/restapi/v1.0/glip/chats", $queryParams);		
	} catch (\RingCentral\SDK\Http\ApiException $e) {
		$response = false ;
 		// $response = $e ;
	}
	
	return $response ;	
}

/* ============================== */
/* Build help icon and title text */
/* ============================== */
function ringcentral_build_help($field) {
    global $wpdb;
    $image_source = RINGCENTRAL_PLUGINURL . 'images/question_mark.png' ;

    $result_rc_help = $wpdb->get_row( $wpdb->prepare("SELECT ringcentral_help_help AS help_text
            FROM `ringcentral_help` WHERE `ringcentral_help_field` = %s", $field) );

    $out_string = "<img src='$image_source' title='" . esc_attr($result_rc_help->help_text) . "' />" ;
    return $out_string ;
}
/* ============================== */
/* Encrypt Password to Obfusicate */
/* ============================== */
function ringcentral_encrypt($plaintext) {
    if (function_exists('openssl_cipher_iv_length')) {
        $key = $_SERVER['SERVER_NAME'].$_SERVER['SERVER_ADDR'].$_SERVER['SERVER_SOFTWARE'];
        $cipher="AES-128-CBC";
        $ivlen = openssl_cipher_iv_length($cipher);
        $iv = substr($c, 0, $ivlen);
        return base64_encode(openssl_encrypt($plaintext, $cipher, $key));
    }
    return $plaintext;
}
/* ============================== */
/* Decrypt Password to Obfusicate */
/* ============================== */
function ringcentral_decrypt($ciphertext) {
    if (function_exists('openssl_cipher_iv_length')) {
        $key = $_SERVER['SERVER_NAME'].$_SERVER['SERVER_ADDR'].$_SERVER['SERVER_SOFTWARE'];
        $cipher="AES-128-CBC";
        return openssl_decrypt(base64_decode($ciphertext), $cipher, $key);
    } else {
        return $ciphertext;
    }
}
/* ============================================ */
/* Get chant name based on provided chat number */
/* ============================================ */
function get_chat_name($chat_num) {
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
                $chat_name = $record->name ;
            }
        }
    } catch (\RingCentral\SDK\Http\ApiException $e) {
        $chat_name = false ;
        // $response = $e ;
    }

    return $chat_name ;
}
/* =============================================================== */
/* Triggered function when access form is submitted on public side */
/* =============================================================== */
function ringcentral_new_access_request($fields) {
    global $wpdb;
//    echo "<pre>" ;
//    var_dump($fields) ;
//    echo "</pre>" ;

    $message = "================================================= \r\n" ;
    $message .= "First Name: " . $fields[0]['first'] . "\r\n" ;
    $message .= "Last Name: " . $fields[0]['last'] . "\r\n" ;
    $message .= "email: " . $fields[1]['value'] . "\r\n" ;
    $message .= "Organization: " . $fields[3]['value'] . "\r\n" ;
    $message .= "Job Function: " . $fields[4]['value'] . "\r\n" ;
    $message .= "Have you published an app?: " . $fields[5]['value'] . "\r\n" ;
    $message .= "App listing URL: " . $fields[8]['value'] . "\r\n" ;
    $message .= "================================================= \r\n \r\n" ;

    // Get chat id from control table.
    $result_rc = $wpdb->get_row( $wpdb->prepare("SELECT `ringcentral_team_chat` FROM `ringcentral_control` WHERE `ringcentral_control_id` = %d", 1) );
    $chatId = $result_rc->ringcentral_team_chat ;

    // post the message to the chat
    post_to_team_messaging($chatId, $message) ;
}
/* ============================================== */
/* Post text to Team Messaging Group Chats (Teams) */
/* ============================================== */
function post_to_team_messaging($chatId, $message) {
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
        $result = true ;
    } catch (\RingCentral\SDK\Http\ApiException $e) {
        // echo "<br/><br/>Failed to add text to selected Team chat -><br/>" . $e ;
        $result = false ;
    }
    return $result;
}
?>