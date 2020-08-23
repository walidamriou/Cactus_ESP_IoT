/*
 ********************************************************************
  Copyright © 2020 Walid Amriou

  Last update: August 2020
 ********************************************************************
*/

#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "tcpip_adapter.h"
#include "esp_eth.h"

#include <wifi.h>

#include <esp_https_server.h>


static const char *TAG = "walid";

const  char * cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEDTCCAvWgAwIBAgIUa6GY/rK79KGjutEqPA9IYP45ZCMwDQYJKoZIhvcNAQEL\n" \
"BQAwgZUxCzAJBgNVBAYTAkRaMQ8wDQYDVQQIDAZNJ3NpbGExDzANBgNVBAcMBk0n\n" \
"c2lsYTEVMBMGA1UECgwMV0FMSUQgQU1SSU9VMRUwEwYDVQQLDAxXQUxJRCBBTVJJ\n" \
"T1UxFDASBgNVBAMMC3dhbGlkYW1yaW91MSAwHgYJKoZIhvcNAQkBFhFjb250YWN0\n" \
"QGdtYWlsLmNvbTAeFw0yMDA4MjMxNzU0NTJaFw0zMDA4MjExNzU0NTJaMIGVMQsw\n" \
"CQYDVQQGEwJEWjEPMA0GA1UECAwGTSdzaWxhMQ8wDQYDVQQHDAZNJ3NpbGExFTAT\n" \
"BgNVBAoMDFdBTElEIEFNUklPVTEVMBMGA1UECwwMV0FMSUQgQU1SSU9VMRQwEgYD\n" \
"VQQDDAt3YWxpZGFtcmlvdTEgMB4GCSqGSIb3DQEJARYRY29udGFjdEBnbWFpbC5j\n" \
"b20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC783sNYdnBX8PI612X\n" \
"v6jE48yP1UzwSfO6Q0RA+kjlffqjUMSUuo4kmfjsnt8LW4LeHcIRGaNjD7qo8PuO\n" \
"lNMADoKFa1Y92xMuXWv2rn1XdDWx6/ggicJ2WAZ8VSjYqSF1MC1TCe8p/B3J7F3S\n" \
"yeQMIpduVz+4tE/BcKxNERHrnQ9mx6pv0a0DUbBYe21qDttTWcoFs3XqMggDFSDp\n" \
"7TRFxa6EiSTctz5B+7BVxyIsSMmFSxy0mDIoKQZBov5DvgePTQnHkYKb/q03dovv\n" \
"jy6dveYZER30VqOC06LS5TXDcIHcnRnUnXWUJBBZoil4cHYby87WNOxokVYV4GdH\n" \
"p3hzAgMBAAGjUzBRMB0GA1UdDgQWBBSsT0zogwBidsthY+37L0SJGnJxCjAfBgNV\n" \
"HSMEGDAWgBSsT0zogwBidsthY+37L0SJGnJxCjAPBgNVHRMBAf8EBTADAQH/MA0G\n" \
"CSqGSIb3DQEBCwUAA4IBAQBfgevm+5qUvFYy/KfCNBWqqIxO7GqavRsgd2oFbno/\n" \
"1EhMun2m60MUrkEFf18hXrnyseAFqt2PbASinXh9ZKX0MFmBcBZ0jAA/ivKRD+ma\n" \
"oFbtvfzwQ3Otm8QD+vgGypZYEYxxl5Ig+Qx98Gs/lYIrfFkmPrSbB8e9qjIDC+0p\n" \
"wmzjOXoCVL2ZGq1Ncv4tZ6OdYgisIm9NXTWKEf1lqfLKIAmkde62RRJ+4cF5+3fZ\n" \
"mTdlxaJdgGSZ7cSzdDSiuCk7dh3HuUfc2/2xmzfxIZflZsWjkuFY98MFdEVNLdOz\n" \
"rYRXGaOciUtAS949pVtq1fYlMUJeV+rwHCFrJinfijGw\n" \
"-----END CERTIFICATE-----\n";

const char * prvtkey = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC783sNYdnBX8PI\n" \
"612Xv6jE48yP1UzwSfO6Q0RA+kjlffqjUMSUuo4kmfjsnt8LW4LeHcIRGaNjD7qo\n" \
"8PuOlNMADoKFa1Y92xMuXWv2rn1XdDWx6/ggicJ2WAZ8VSjYqSF1MC1TCe8p/B3J\n" \
"7F3SyeQMIpduVz+4tE/BcKxNERHrnQ9mx6pv0a0DUbBYe21qDttTWcoFs3XqMggD\n" \
"FSDp7TRFxa6EiSTctz5B+7BVxyIsSMmFSxy0mDIoKQZBov5DvgePTQnHkYKb/q03\n" \
"dovvjy6dveYZER30VqOC06LS5TXDcIHcnRnUnXWUJBBZoil4cHYby87WNOxokVYV\n" \
"4GdHp3hzAgMBAAECggEAI3m3ljUL9vXubHQQfF4OquscZ2GpXgl3WYbs0R7st3Rp\n" \
"5H2QPUphHgN5+p+aFfi14NrZEIU4U7BaWRzzPlxU30Pd2vUBmDdLjYrQoAUbX3Bt\n" \
"2iFh+jJGZDQFNV6VkR+LUyU+yNetLs2Jid11cB3vT2MQFiGdw201SJvNydYY/dGa\n" \
"iC0yn89H1BBXnBjYPX3CBe0g5RRSh3wRcPEjn2tCOe1yRg0xpCjBvWvqwRebf4s3\n" \
"Q6e+92CHuqq/pC4YJcCHzglQSg6cPsfNgxkeLaNp6v1RaxmmwMbUurQ6Q87gDQhH\n" \
"U+ikDf0uC6wFP23V5p6KnhAMYLXIHI16fcZmPk51YQKBgQDiN/eJZZhw9gzYREa9\n" \
"WtdRzLoNC5pzPMm+lZIDkWZfPKF7Uf71loPHxHWts1/Ct2FOCJ9Yx2tgNZ+j6mAQ\n" \
"qTlSWsEKo7OWcJr5PshtA8YMtdZpSX5WaN25FnX/gIiOz2SPYMV5VNV3PiPDZEB0\n" \
"k+IozHqargvBe7+idG7M03mgiwKBgQDUsdHEorU/dZxZqdKrC1OG91kGuv5SW6Ya\n" \
"4k3VWzLmT/eeSPFrOJNIqtpm3HeV8a6DKudlaKc67iOucE7iGafmHfdGI0TQHo6z\n" \
"nyO53wu9JzTBTar+9TEyPtEDuCirLVMB1fMxmd+AqCjtUQoDm6h5/8A+x03H/rVT\n" \
"3loicsHcuQKBgQCWrTiPp7/13a2Ij1Ge5lbtATQbZFMn1qF59B4hikNpw/S7M05j\n" \
"PjHgaZcIb6WlCIgjddLNS2bV2mHOyFncPxsTI6fY8li4J6DRy2OsWQsdhDCgSHeC\n" \
"Dq9AjRljabWLidAtQAPIYR0eI6Gmj6vu+mF1U12Gkc76vPZTusHiZClrKQKBgGJb\n" \
"AGfv3eTEnFKSR4s1J6EIDLEiE60+GGfiMp/xFm6OmDEoBgvRE7rM10YtuVcrdcnO\n" \
"M0cfyAVmTjXCVZgUySGOxeQ3NwEOwGqrsrenPyRvVuP06JgSBnf3uGwB4zyVLwdw\n" \
"So+rWkGCgbqMeRcGCjiwXdQqEfvj+zIXZpL7gk5xAoGATorhbaQPvAiO+TXXVQTR\n" \
"QCjEX6QAYQ+k2GeuPKTEZzAvUCkuTt5fyVSkVbflawv2tbKyfFe73IuXYH7XOLW6\n" \
"02prLVu0PyLhpIWMdmRjh4T+y8URtTTcPyckYIPQxo8x9hzam2f6uViy+tLxCvbJ\n" \
"kWEFF8/XT059QHtV4JGi1Xw=\n" \
"-----END PRIVATE KEY-----\n";

/* An HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h4>ESP32 Walid Amriou</h4>", -1); // -1 = use strlen()

    return ESP_OK;
}

static const httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler
};


static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server");

    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();

    /*
    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
    extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
    conf.cacert_pem = cacert_pem_start;
    conf.cacert_len = cacert_pem_end - cacert_pem_start;
    */
    conf.cacert_pem = (uint8_t *)cert;
    conf.cacert_len = 1465;

    /*
    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
    extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
    conf.prvtkey_pem = prvtkey_pem_start;
    conf.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;
    */
    conf.prvtkey_pem = (uint8_t *)prvtkey;
    conf.prvtkey_len = 1705;


    esp_err_t ret = httpd_ssl_start(&server, &conf);
    if (ESP_OK != ret) {
        ESP_LOGI(TAG, "Error starting server!");
        return NULL;
    }

    // Set URI handlers
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server, &root);
    return server;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_ssl_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        *server = start_webserver();
    }
}

void app_main()
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);  

    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* Register event handlers to start server when Wi-Fi or Ethernet is connected,
     * and stop server when disconnection happens.
     */

    wifi_init();
    printf("wifi init done.\n");
    wifi_connect();
    printf("Connected to wifi network\n");

    // ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    // ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
    
    start_webserver();
    printf("Start WebServer\n");

}
