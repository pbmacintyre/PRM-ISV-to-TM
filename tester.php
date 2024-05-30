<?php

require('includes/vendor/autoload.php');

show_errors();

$queryParams = array(
    'type' => array( 'Team' ),

);
$server = "https://platform.ringcentral.com";
$client_id = "4Wlqrn8Zjw9bZrvUfQqwFQ";
$client_secret = "Ag2tTShoKr1e8S4bnGY7gt9q1WDGTdeTbfnYc52eRJhI";
$jwt = "eyJraWQiOiI4NzYyZjU5OGQwNTk0NGRiODZiZjVjYTk3ODA0NzYwOCIsInR5cCI6IkpXVCIsImFsZyI6IlJTMjU2In0.eyJhdWQiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbS9yZXN0YXBpL29hdXRoL3Rva2VuIiwic3ViIjoiNjIxOTk0ODYwMTYiLCJpc3MiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbSIsImV4cCI6Mzg2NDU4NTQ3NSwiaWF0IjoxNzE3MTAxODI4LCJqdGkiOiJrYzlSTUluclJWbUZUYzUzLV9zVE1nIn0.RM2UOCBFfoLOzviuGGu47RLBxRohnFqeEO3IUwOhUEBHtJstf4n6VioeNSznidC58qoKcMb9BxoyxgWkVSz8Py-ucisxdVksDPdQdVbjSr-bvmJUox4iXNrSfzbBCilTjFB073kKhFuopQbeL2IG6dkp9-EuQ9nUhSddqj2Zq9yPxTJ2O_kbk8DbK0jMaCgzoNyA3l5E2imk8uZrfr324yIGXf8Lp1D0eopS5M_c2QrORXMbOU06IhY4aumXQpgU-LLkV3ogKretQ6UJHkhvhJKvW_ZmqGSBOcBhmTBINejS7vzhkulFA47QOmp3IYmLKwU8GAGg33rR3xssI7RwAw";

$rcsdk = new RingCentral\SDK\SDK($client_id, $client_secret, $server);
$platform = $rcsdk->platform();
$platform->login(["jwt" => $jwt]);

try {
    $r = $platform->get("/team-messaging/v1/chats", $queryParams);
    echo_spaces("chat groups", $r);
} catch (Exception $e) {
    echo_spaces("Error MesSaGe", $e->getMessage());
}

function echo_spaces($text, $value="", $lines=0) {
    echo "<br /><strong><p style='color: red; display: inline'>$text:</p></strong> " ;
    if (is_string($value) || is_int($value))  { echo $value; }
    if (is_object($value) || is_array($value)) { ringcentral_dump_obj($value); }
    if (is_bool($value)) { if ($value) echo "TRUE / Success"; else echo 'FALSE / Failure'; }
    $i = 1 ;
    while ($i <= $lines) {
        echo "<br />" ;
        $i++;
    }
}
function show_errors() {
    error_reporting();
//     error_reporting(E_ALL & ~E_NOTICE & ~E_WARNING);
    ini_set('display_errors', 1);
}
function ringcentral_dump_obj($object) {
    echo "<pre>";
    var_dump($object);
    echo "</pre>";
}