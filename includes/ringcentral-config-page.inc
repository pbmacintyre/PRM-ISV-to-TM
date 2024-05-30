<?php
/**
 * Copyright (C) 2019-2022 Paladin Business Solutions
 *
 */

/* ========= */
/* show_form */
/* ========= */
Function show_form($message, $label = "", $color = "#008EC2") {	
	
	global $print_again, $wpdb;  ?>

	<form action="" method="post" >
	<table class="TableOverride" >
    <tr class="TableOverride">
		<td colspan="2" style="text-align: center;">
    <?php
	if ($print_again == true) {
		echo "<font color='$color'><strong>" . esc_html($message) . "</strong></font>";
	} else {
	    echo "<font color='$color'><strong>" . esc_html($message) . "</strong></font>";	    
	}
	
	$result_rc = $wpdb->get_row( $wpdb->prepare("SELECT * FROM `ringcentral_control` WHERE `ringcentral_control_id` = %d", 1) );

//    $response = list_tm_teams() ;
    $response = "" ;
    $sdk = ringcentral_sdk();
    echo_spaces("SDK Connection", $sdk);

    if ($response == false) {
        $message = "Failed to retrieve Team names";
        $color = "red" ;
    }

    ?>
        </td>
	</tr>
    <tr class="TableOverride">
        <td class="left_col">
            <p style='display: inline; <?php if ($label == "chat_id") echo "color:red"; ?>' >Available Team Chats:</p>
            <?php echo ringcentral_build_help("team_chats") ; ?>
        </td>
        <td class="right_col">
            <?php if ($response == false) {
                echo "<font color='red'>No Team Chats are currently available</font>" ;
            } else { ?>
                <select name="chat_id">
                    <?php
                    if ($print_again) {
                        if ($_POST['chat_id']) {
                            $chat_name = get_chat_name($_POST['chat_id']) ;  ?>
                            <option selected value="<?= $_POST['chat_id'] ?>"><?= $chat_name ?> </option>
                <?php   } else {  ?>
                            <option selected value="-1">Choose Team to Post Chat into</option>
                <?php   }
                        foreach ($response->json()->records as $record) { ?>
                            <option value="<?= $record->id ; ?>"><?= $record->name ; ?></option>
            <?php       }
                    } else {
                        $chat_name = get_chat_name($result_rc->ringcentral_team_chat) ;   ?>
                        <option selected value="<?= $result_rc->ringcentral_team_chat ?>"><?= $chat_name ?> </option>
            <?php       foreach ($response->json()->records as $record) {  ?>
                            <option value="<?= $record->id ; ?>" > <?= $record->name ; ?> </option>
            <?php       }
                    }   ?>
                </select>
            <?php } ?>
        </td>
    </tr>
    <tr>
        <td colspan=2>
        	<hr>
        </td>
    </tr>	
	<?php /* ============================================== */ ?>
	<tr class="TableOverride">
        <td class="left_col">
            <p style='display: inline;' >Reveal Passwords:</p>
        </td>
        <td class="right_col">             
            <button type="button" onmouseout="hide_it();" onmousedown="reveal_it();" onmouseup="hide_it();">Toggle Passwords</button>     
        </td>
    </tr>   
    <tr class="TableOverride">
		<td class="left_col">
			<p style='display: inline; <?php if ($label == "client_id") echo "color:red"; ?>' >Client ID:</p>
			<p style='color: red; display: inline'>*</p>
		</td>
		<td class="right_col"><input type="text" name="client_id" value="<?php 
		  if ($print_again) {
		      echo sanitize_text_field($_POST['client_id']);
          } else {             
              if ($result_rc->client_id) {
                  echo $result_rc->client_id ;
              } else {
                echo "&nbsp;" ;  
              }
          }
            ?>"></td>
	</tr>	
	<tr class="TableOverride">
		<td class="left_col">
			<p style='display: inline; <?php if ($label == "client_secret") echo "color:red"; ?>' >Client Secret Key:</p>
			<p style='color: red; display: inline'>*</p>
		</td>
		<td class="right_col"><input type="password" name="client_secret" id="myClientSecret" value="<?php 
		  if ($print_again) {
		      echo sanitize_text_field($_POST['client_secret']);
          } else {             
              if ($result_rc->client_secret) {
                echo $result_rc->client_secret ;
              } 
          }
            ?>" > 
            </td>
	</tr>
        <tr class="TableOverride">
            <td class="left_col">
                <p style='display: inline; <?php if ($label == "client_secret") echo "color:red"; ?>' >JWT Key:</p>
                <p style='color: red; display: inline'>*</p>
            </td>
            <td class="right_col"><textarea name="jwt_key"><?php
                    if ($print_again) {
                        echo strip_tags($_POST['jwt_key']);
                    }
                    ?></textarea>
            </td>
        </tr>
        <tr class="TableOverride">
		<td colspan="2" align="center">			
			<br/>
			<?php 
			$btn_attributes = array( "style" => "background: #008ec2; border-color: #006799; color: #fff;" );
			submit_button("Save Settings","","submit","",$btn_attributes); ?>
			<br/><br/>
		</td>
	</tr>
	</table>
	</form>
<?php
}
/* ========== */
/* check_form */
/* ========== */
Function check_form() {
	
	global $print_again, $wpdb;
	
	$label = "" ;
	$message = "" ;

	$rc_handshake = array(
	    "client_id"     => sanitize_text_field($_POST['client_id']),
	    "client_secret" => sanitize_text_field($_POST['client_secret']),
	    "jwt_key"       => sanitize_text_field($_POST['jwt_key']) ,

	);

// 	/* data integrity checks */	
	if (ringcentral_validate_sdk($rc_handshake) == false) {
	    $print_again = true;
	    $label = "";
	    $message = "There was an error validating your Ring Central account credentials, please verify and try again";
	}
	if ($rc_handshake["client_id"] == "") {
	    $print_again = true;
	    $label = "client_id";
	    $message = "Client ID cannot be blank.";
	}	
	if ($rc_handshake["client_secret"] == "") {
	    $print_again = true;
	    $label = "client_secret";
	    $message = "Client Secret Key cannot be blank.";
	}
	if ($rc_handshake["user_name"] == "") {
	    $print_again = true;
	    $label = "ringcentral_user_name";
	    $message = "RingCentral User Name cannot be blank.";
	}
	if ($rc_handshake["extension"] == "") {
	    $print_again = true;
	    $label = "ringcentral_extension";
	    $message = "RingCentral extension cannot be blank.";
	}
	if ($rc_handshake["password"] == "") {
	    $print_again = true;
	    $label = "ringcentral_password";
	    $message = "RingCentral password cannot be blank.";
	}

	// end data integrity checking

	if ($print_again == true) {
		$color = "red" ;
	    show_form($message, $label, $color);
	} else {	    
	    /* convert and / or sanitize any needed form POST values */
	    if ($_POST['embedded_phone'] == "on") { 
	        $embedded_phone = 1;	        
        } else {
            $embedded_phone = 0;
        }

        $chat_id = $_POST['chat_id'] ;

//		$rc_handshake['password'] = $rc_handshake['password'];
		$rc_handshake['password'] = ringcentral_encrypt($rc_handshake['password']);
//      1q2w3e4r5t!!Q

        $config_sql = $wpdb->prepare("UPDATE `ringcentral_control` SET
            `embedded_phone` = %d, `client_id` = %s, `client_secret` = %s,
            `ringcentral_user_name` = %s, `ringcentral_extension` = %d, `ringcentral_password` = %s,
            `ringcentral_team_chat` = %d, `ringcentral_sandbox` = %d        	
            WHERE `ringcentral_control_id` = %d", $embedded_phone, $rc_handshake['client_id'], $rc_handshake['client_secret'],
            $rc_handshake['user_name'], $rc_handshake['extension'], $rc_handshake['password'],
            $chat_id, $sandbox, 1) ;
                
        $wpdb->query( $config_sql ); 
        
	    $color = "#53DF00";
	    $message = "Configuration Changes have been saved";	   
	    show_form($message, $label, $color) ;	    
	}
}
/* ============= */
/*  --- MAIN --- */
/* ============= */
if(isset($_POST['submit'])) {
	check_form();
} else {
	$message = "Adjust settings to your preferences";
	show_form($message);
} 
?>