#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <json/json.h> // Include jsoncpp library

using namespace std;

// Function to convert a number from given base to decimal
long long baseToDecimal(string value, int base) {
    long long decimalValue = 0;
    for (char digit : value) {
        decimalValue *= base;
        if (isdigit(digit))
            decimalValue += digit - '0';
        else
            decimalValue += digit - 'a' + 10; // Handles hexadecimal or bases > 10
    }
    return decimalValue;
}

// Function to perform Lagrange interpolation and find constant term
long long findConstantTerm(vector<pair<int, long long>>& points, int k) {
    double c = 0;
    for (int i = 0; i < k; i++) {
        double term = points[i].second; // y_i
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - points[j].first) / (double)(points[i].first - points[j].first);
            }
        }
        c += term;
    }
    return round(c); // Return the constant term rounded to nearest integer
}

int main() {
    string filename1 = "testcase1.json";
    string filename2 = "testcase2.json";

    vector<string> filenames = {filename1, filename2};

    for (const string& filename : filenames) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file: " << filename << endl;
            return 1;
        }

        Json::Value root;
        file >> root;
        file.close();

        int n = root["keys"]["n"].asInt();
        int k = root["keys"]["k"].asInt();

        vector<pair<int, long long>> points;

        // Extract points (x, y) from JSON
        for (auto it = root.begin(); it != root.end(); ++it) {
            if (it.key().asString() == "keys") continue;

            int x = stoi(it.key().asString()); // x value
            int base = it->operator[]("base").asInt();
            string value = it->operator[]("value").asString();

            long long y = baseToDecimal(value, base); // Convert y to decimal

            points.push_back({x, y});
        }

        // Solve for the constant term (c)
        long long secret = findConstantTerm(points, k);
        cout << "Secret for " << filename << ": " << secret << endl;
    }

    return 0;
}