<?php
/*
Plugin Name: A PRM to TM
Plugin URI:  https://ringcentral.com/
Description: RingCentral Plugin for sending notices to Team Messaging app
Author:      Peter MacIntyre
Version:     1.8
Author URI:  https://paladin-bs.com/peter-macintyre/
Details URI: https://paladin-bs.com
License:     GPL2
License URI: https://www.gnu.org/licenses/gpl-2.0.html
Copyright (C) 2022 Paladin Business Solutions

This plugin sends a message to the Team Messaging Application into a Group
telling the Group that a new PRM Access Request form has been submitted.

PRM to TM is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

See License URI for full details.

*/

//error_reporting(E_ALL & ~(E_WARNING | E_NOTICE));
//ini_set('display_errors', 1);

/* ============================== */
/* Set RingCental Constant values */
/* ============================== */
if (!defined('RINGCENTRAL_PLUGINDIR')) {
    define('RINGCENTRAL_PLUGINDIR', plugin_dir_path(__FILE__));
}
if (!defined('RINGCENTRAL_PLUGINURL')) {
    define('RINGCENTRAL_PLUGINURL', plugin_dir_url(__FILE__));
    //  http path returned
}
if (!defined('RINGCENTRAL_PLUGIN_INCLUDES')) {
    define('RINGCENTRAL_PLUGIN_INCLUDES', plugin_dir_path(__FILE__) . "includes/");
}
if (!defined('RINGCENTRAL_PLUGIN_FILENAME')) {
    define('RINGCENTRAL_PLUGIN_FILENAME', plugin_basename(dirname(__FILE__) . '/prm-to-tm.php'));
}
if (!defined('RINGCENTRAL_LOGO')) {
    define('RINGCENTRAL_LOGO', RINGCENTRAL_PLUGINURL . 'images/ringcentral-logo.png');
}
/* ================================= */
/* set ring central supporting cast  */
/* ================================= */
function ringcentral_js_add_script () {
    $js_path = RINGCENTRAL_PLUGINURL . 'js/ringcentral-scripts.js';
    wp_enqueue_script('ringcentral-js', $js_path);
}

add_action('init', 'ringcentral_js_add_script');

function ringcentral_js_add_admin_script () {
    $js_path = RINGCENTRAL_PLUGINURL . 'js/ringcentral-admin-scripts.js';
    wp_enqueue_script('ringcentral-admin-js', $js_path);
}

add_action('admin_enqueue_scripts', 'ringcentral_js_add_admin_script');

function ringcentral_load_custom_admin_css () {
    wp_register_style('ringcentral_custom_admin_css',
        RINGCENTRAL_PLUGINURL . 'css/ringcentral-custom.css',
        false, '1.0.0');
    wp_enqueue_style('ringcentral_custom_admin_css');
}

add_action('admin_print_styles', 'ringcentral_load_custom_admin_css');

/* ====================================== */
/* bring in generic ringcentral functions */
/* ====================================== */
require_once("includes/ringcentral-functions.inc");

/* ========================================= */
/* Make top level menu                       */
/* ========================================= */
function ringcentral_menu () {
    add_menu_page(
        'PRM to TM',                        // Page & tab title
        'PRM to TM',                        // Menu title
        'manage_options',                   // Capability option
        'ringcentral_Admin',                // Menu slug
        'ringcentral_config_page',   // menu destination function call
        RINGCENTRAL_PLUGINURL . 'images/ringcentral-icon.png', // menu icon path
        15                                       // menu position level
    );
    add_submenu_page(
        'ringcentral_Admin',               // parent slug
        'PRM to TM: Configurations',       // page title
        'Settings',                        // menu title - can be different than parent
        'manage_options',                  // options
        'ringcentral_Admin'                // menu slug to match top level (go to the same link)
    );
}

/* ========================================= */
/* page / menu calling functions             */
/* ========================================= */
// call add action func on menu building function above.
add_action('admin_menu', 'ringcentral_menu');
// function for default Admin page
function ringcentral_config_page () {
    // check user capabilities
    if (!current_user_can('manage_options')) {
        return;
    }
    ?>
    <div class="wrap">
        <img id='page_title_img' title="RingCentral Plugin" src="<?= RINGCENTRAL_LOGO; ?>">
        <h1 id='page_title'><?= esc_html(get_admin_page_title()); ?></h1>

        <?php require_once(RINGCENTRAL_PLUGINDIR . "includes/ringcentral-config-page.inc"); ?>

    </div>
    <?php
}

/* ========================================= */
/* Add action hook for sending note to Team
/* Messaging on request form submission
/* ========================================= */
add_action('wpforms_process_entry_save', 'ringcentral_new_access_request');

/* ============================================= */
/* Add registration hook for plugin installation */
/* ============================================= */
function ringcentral_install () {
    require_once(RINGCENTRAL_PLUGINDIR . "includes/ringcentral-install.inc");
}

/* ============================================= */
/* Create default materials on plugin activation */
/* ============================================= */
function ringcentral_activate_plugin () {
    require_once(RINGCENTRAL_PLUGINDIR . "includes/ringcentral-activation.inc");
}

register_activation_hook(__FILE__, 'ringcentral_install');
register_activation_hook(__FILE__, 'ringcentral_activate_plugin');