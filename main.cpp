#include <spdlog/spdlog.h>
#include <iostream>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <string>


typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;


void on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    spdlog::info("hdl: {} message: {}", hdl.lock().get(), msg->get_payload());
}

void on_open(client *c, websocketpp::connection_hdl hdl) {
    spdlog::info("hdl: {} open", hdl.lock().get());
}

static context_ptr on_tls_init() {
    // establishes a SSL connection
    context_ptr ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 | boost::asio::ssl::context::single_dh_use);
    } catch (std::exception &e) {
        spdlog::error("Error in context pointer: {}", e.what());
        spdlog::error("Error in context pointer: {}", e.what());
    }
    return ctx;
}


int main() {
    client c;

    std::string uri_binance = "wss://stream.binance.com:9443/ws/btcusdt@bookTicker";
    try {
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        c.init_asio();
        c.set_tls_init_handler(bind(&on_tls_init));
        c.set_open_handler(bind(&on_open, &c, ::_1));
        c.set_message_handler(bind(&on_message, ::_1, ::_2));

        websocketpp::lib::error_code ec;
        client::connection_ptr con_binance = c.get_connection(uri_binance, ec);
        if (ec) {
            spdlog::error("could not create connection because: {}", ec.message());
            return 0;
        }
        c.connect(con_binance);
        c.run();
    } catch (websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
        spdlog::error("Some error message with arg: {}", e.what());
    }
}