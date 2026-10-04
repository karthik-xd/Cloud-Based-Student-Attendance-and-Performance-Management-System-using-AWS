#include "auth.hpp"
#include <iostream>
#include <cassert>

void testPasswordHashing() {
    AuthManager& auth = AuthManager::getInstance();
    std::string pass = "Admin@123";
    std::string hash = auth.hashPassword(pass, "univ2026");

    assert(!hash.empty());
    assert(auth.verifyPassword("Admin@123", hash));
    assert(!auth.verifyPassword("WrongPassword", hash));

    std::cout << "[TEST PASSED] Password Hashing & Verification Test" << std::endl;
}

int main() {
    std::cout << "[RUNNING TESTS] Starting Authentication Tests..." << std::endl;
    testPasswordHashing();
    std::cout << "[ALL TESTS PASSED] Authentication Test Suite Completed Successfully." << std::endl;
    return 0;
}
