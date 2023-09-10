#pragma once

#include "esp_err.h"
#include "esp_http_server.h"

enum HTTP_SERVER_STATUS : bool
{
  /**
   * @brief Server is stopped/notStarted
   */
  STOPPED,

  /**
   * @brief Server is started/working
   */
  STARTED
};

class HttpServer
{
public:
  HttpServer() {}

  /**
   * @brief Start the http server and add endpoint handlers
   * @return esp_err_t ESP_OK if normal started
   */
  esp_err_t Start()
  {
    return http_app_start();
  }

  /**
   * @brief Stop http server
   * @return esp_err_t
   */
  esp_err_t Stop()
  {
    return http_app_stop();
  }

  /**
   * @brief change the default config
   * @param config
   */
  virtual void Config(httpd_config_t *config)
  {
  }

  /**
   * @brief add http endpoint handlers
   */
  virtual void RegisterUriHandlers()
  {
  }

  /**
   * @brief Get the Status
   * @return HTTP_SERVER_STATUS
   */
  HTTP_SERVER_STATUS GetStatus()
  {
    return status;
  }

protected:
  httpd_handle_t httpd_handle = nullptr;

private:
  HTTP_SERVER_STATUS status = STOPPED;

  esp_err_t http_app_start()
  {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    Config(&config);

    esp_err_t err = httpd_start(&httpd_handle, &config);

    if (err == ESP_OK)
    {
      RegisterUriHandlers();
      status = STARTED;
    }

    return err;
  }

  esp_err_t http_app_stop()
  {
    if (httpd_handle == nullptr)
      return ESP_FAIL;

    httpd_stop(httpd_handle);
    httpd_handle = nullptr;
    status = STOPPED;
    return ESP_OK;
  }
};
