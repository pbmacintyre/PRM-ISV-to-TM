<?php

require('includes/vendor/autoload.php');
require('includes/ringcentral-functions.inc');

show_errors();

$server = 'https://platform.ringcentral.com';
$client_id = "4Wlqrn8Zjw9bZrvUfQqwFQ";
$client_secret = "Ag2tTShoKr1e8S4bnGY7gt9q1WDGTdeTbfnYc52eRJhI";
$jwt = "eyJraWQiOiI4NzYyZjU5OGQwNTk0NGRiODZiZjVjYTk3ODA0NzYwOCIsInR5cCI6IkpXVCIsImFsZyI6IlJTMjU2In0.eyJhdWQiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbS9yZXN0YXBpL29hdXRoL3Rva2VuIiwic3ViIjoiNjIxOTk0ODYwMTYiLCJpc3MiOiJodHRwczovL3BsYXRmb3JtLnJpbmdjZW50cmFsLmNvbSIsImV4cCI6Mzg2NDU4NTQ3NSwiaWF0IjoxNzE3MTAxODI4LCJqdGkiOiJrYzlSTUluclJWbUZUYzUzLV9zVE1nIn0.RM2UOCBFfoLOzviuGGu47RLBxRohnFqeEO3IUwOhUEBHtJstf4n6VioeNSznidC58qoKcMb9BxoyxgWkVSz8Py-ucisxdVksDPdQdVbjSr-bvmJUox4iXNrSfzbBCilTjFB073kKhFuopQbeL2IG6dkp9-EuQ9nUhSddqj2Zq9yPxTJ2O_kbk8DbK0jMaCgzoNyA3l5E2imk8uZrfr324yIGXf8Lp1D0eopS5M_c2QrORXMbOU06IhY4aumXQpgU-LLkV3ogKretQ6UJHkhvhJKvW_ZmqGSBOcBhmTBINejS7vzhkulFA47QOmp3IYmLKwU8GAGg33rR3xssI7RwAw";

$sdk = new RingCentral\SDK\SDK($client_id, $client_secret, $server);
$rcsdk = $sdk->platform();
// Login via API
if (!$rcsdk->loggedIn()) {
    try {
        $rcsdk->login(["jwt" => $jwt]);
    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        $rcsdk = $e->getMessage();
    }
}

$chatId = '136080719878' ;

try {
    $r = $rcsdk->get("/team-messaging/v1/chats/{$chatId}");
    echo_spaces("chat details", $r->json());
    echo_spaces("Team ID / Name", $r->json()->id . " => " . $r->json()->name ) ;

} catch (Exception $e) {
    echo_spaces("Error", $e->getMessage());
}

