#include "mbed.h"
#include "easy-connect.h"
#include "https_request.h"

Serial pc(USBTX, USBRX);

const char SSL_CA_PEM[] = "-----BEGIN CERTIFICATE-----\n"
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
    "-----END CERTIFICATE-----\n";

    
const char spotify_access_token[] = "BQAR8281IqRecDrC4XmLKN4IMgTfcvb-LWD_7b_8OanszwFprT-ggg553P7oFta6_WWPwDy7U61aliqDyR3JjU_cQfkUUmGonfP7G4_GP4SwfsabBog0g3Ik-asdVsYFBWDH8pIrO0QDibE731Lu0PeuTCM";

void dump_response(HttpResponse* res) {
    pc.printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());

    pc.printf("Headers:\n");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++) {
        pc.printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    pc.printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
}

void run() {
    NetworkInterface* network = easy_connect(true);
    if (!network) {
        return;
    }

    // Create a TLS socket (which holds a TCPSocket)
    pc.printf("\n----- Setting up TLS connection -----\n");
    
    pc.printf("TLS socket set up, now starting request!");
    HttpsRequest* play_req = new HttpsRequest(network, SSL_CA_PEM, HTTP_PUT, "https://api.spotify.com/v1/me/player/play");
    play_req->set_debug(true);
    play_req->set_header("Authorization: ", strcat("Bearer ", spotify_access_token));

    HttpResponse* get_res = play_req->send();
    if (!get_res) {
        pc.printf("HttpRequest failed (error code %d)\n", play_req->get_error());
        return;
    }
    pc.printf("\n----- HTTPS GET response -----\n");
    dump_response(get_res);

    delete play_req;

    delete socket;
}

int main() {
    pc.baud(115200);
    run();
}
