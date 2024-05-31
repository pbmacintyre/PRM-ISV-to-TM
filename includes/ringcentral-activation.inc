<?php 
/*
 * Copyright (C) 2019-2022 Paladin Business Solutions
 *
 */

global $wpdb;
require_once (ABSPATH . 'wp-admin/includes/upgrade.php');
/* ================================= */
/* Create ringcentral_control table  */
/* ================================= */

$ringcentral_sql = "CREATE TABLE `ringcentral_control` (
  `ringcentral_control_id` int(11) NOT NULL AUTO_INCREMENT,
  `client_id` varchar(50) NOT NULL,  
  `client_secret` varchar(50) NOT NULL,
  `jwt_key` text NOT NULL,        
  `chat_id` varchar(50) NOT NULL,    
  PRIMARY KEY (`ringcentral_control_id`)
) ";
dbDelta($ringcentral_sql);
/* ====================================== */
/* seed table with control row and basic  */
/* data if there is no existing row       */
/* ====================================== */
$row_exists = $wpdb->get_var($wpdb->prepare("SELECT `ringcentral_control_id` 
    FROM `ringcentral_control` WHERE `ringcentral_control_id` = %d", 1) );
if (!$row_exists) {
    $wpdb->insert('ringcentral_control',
        array(
            'ringcentral_control_id' => 1,
            )
        );
}
/* ==============================*/
/* Create ringcentral_help table */
/* ==============================*/
$ringcentral_sql = "CREATE TABLE `ringcentral_help` ( 
`ringcentral_help_id` INT NOT NULL AUTO_INCREMENT , 
`ringcentral_help_field` VARCHAR(75) NULL , 
`ringcentral_help_help` TEXT NULL,  
PRIMARY KEY (`ringcentral_help_id`))";
dbDelta($ringcentral_sql);
/* ====================================== */
/* seed table with control row and basic  */
/* data if there is no existing row       */
/* ====================================== */
$row_exists = $wpdb->get_var($wpdb->prepare("SELECT `ringcentral_help_id` FROM `ringcentral_help`
        WHERE `ringcentral_help_id` = %d", 1) );
// should check all rows individually.
// clean DB table and re-seed the table
$wpdb->query( "truncate `ringcentral_help`") ;
$wpdb->query( $wpdb->prepare("INSERT INTO `ringcentral_help`
  (`ringcentral_help_id`, `ringcentral_help_field`, `ringcentral_help_help`)
  VALUES (%d, %s, %s)",
    1, "username", "Main Company Phone Number" ) );
$wpdb->query( $wpdb->prepare("INSERT INTO `ringcentral_help`
  (`ringcentral_help_id`, `ringcentral_help_field`, `ringcentral_help_help`)
  VALUES (%d, %s, %s)",
    2, "ringcentral_sandbox", "Turn this checkbox ON if you want to use the RingCentral SANDBOX SDK, leave it off for Production use." ) );
$wpdb->query( $wpdb->prepare("INSERT INTO `ringcentral_help`
  (`ringcentral_help_id`, `ringcentral_help_field`, `ringcentral_help_help`)
  VALUES (%d, %s, %s)",
    3, "team_chats", "This will show all the teams to which you currently have posting permissions, select the team name that you want to use from this list." ) );
$wpdb->query( $wpdb->prepare("INSERT INTO `ringcentral_help`
  (`ringcentral_help_id`, `ringcentral_help_field`, `ringcentral_help_help`)
  VALUES (%d, %s, %s)",
    4, "embedded_phone", "Use this checkbox to turn on the Embedded Phone tool, note it cannot be on at the same time as embedded Team Messaging" ) );

?>