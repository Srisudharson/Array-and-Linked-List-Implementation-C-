#ifndef DATASET_HANDLER_HPP
#define DATASET_HANDLER_HPP
#include <string>
#include <iostream>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <limits> 
#include <iomanip> 
#include <algorithm> 
using json = nlohmann::json;
using namespace std;

// Define a struct to hold transaction data
struct Transaction {
    string transaction_id;
    string timestamp;
    string sender_account;
    string receiver_account;
    double amount;
    string transaction_type;
    string merchant_category;
    string location;
    string device_used;
    bool is_fraud;
    string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    double velocity_score;
    double geo_anomaly_score;
    string payment_channel;
    string ip_address;
    string device_hash;

    // Default constructor
    Transaction() : amount(0.0), is_fraud(false), time_since_last_transaction(0.0),
                    spending_deviation_score(0.0), velocity_score(0.0), geo_anomaly_score(0.0) {}
};

// Function to convert Transaction struct to JSON object
void to_json(nlohmann::json& j, const Transaction& t) {
    j = nlohmann::json{
        {"transaction_id", t.transaction_id},
        {"timestamp", t.timestamp},
        {"sender_account", t.sender_account},
        {"receiver_account", t.receiver_account},
        {"amount", t.amount},
        {"transaction_type", t.transaction_type},
        {"merchant_category", t.merchant_category},
        {"location", t.location},
        {"device_used", t.device_used},
        {"is_fraud", t.is_fraud},
        {"fraud_type", t.fraud_type},
        {"time_since_last_transaction", t.time_since_last_transaction},
        {"spending_deviation_score", t.spending_deviation_score},
        {"velocity_score", t.velocity_score},
        {"geo_anomaly_score", t.geo_anomaly_score},
        {"payment_channel", t.payment_channel},
        {"ip_address", t.ip_address},
        {"device_hash", t.device_hash}
    };
}

#endif