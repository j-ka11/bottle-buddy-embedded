/**
 * @file service.h
 */

#pragma once

#include <stdlib.h>
#include <string>
#include <cstring>
#include <math.h>
#include <unordered_map>
#include <ArduinoBLE.h>
#include "Pipeline/router.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Base class for high level services
     * 
     * To create a new Bottle Buddy service, simply extend this base class. A derived service
     * must implement the receive function, which is called when the derived service receives a package
     * from any of the locations they are subscribed to.
     * 
     * Additionally, this base class handles interfacing with the BLE module once it has reached the 
     * GATT stage of data transmission.
     */
    class Service {
    public:
        Service(const char* uid);

        ~Service();

        /**
         * @brief Used by the router class to deliver a package to a service.
         * 
         * Can be implemented however the derived service needs in order to provide its service.
         */
        virtual void receive(Package package);
    protected:
        /**
         * @brief The service's UID string, which is used for BLE purposes.
         */
        const char* uid;
        /**
         * @brief The services BLEService variable, which creates a new service within the BLE module.
         */
        BLEService bleService;

        /**
         * @brief Vector of characteristic uuids.
         * 
         * These are created dynamically when a derived service requests a new BLE characteristic.
         */
        std::vector<const char*> uuids;
        /**
         * @brief Current number of characteristics this service has created.
         * 
         * Used to dynamically create new UUIDs, as well as enforce a max of 16 characteristics per service.
         */
        int numCharacteristics;
        /**
         * @brief Map of string to BLE characteristics.
         */
        std::unordered_map<std::string, BLECharacteristic> characteristics;

        /**
         * @brief Used to create a new characteristics within the BLE module.
         * 
         * A derived service can call this function which handles interfacing with the BLE module.
         * A max of 16 characteristics can be created per service.
         */
        void createCharacteristic(std::string name, uint8_t properties);
    private:
        /**
         * @brief Used when dynamically creating new characteristic UUIDs.
         */
        std::unordered_map<char, int> hexConversions;

        /**
         * @brief Creates a new characteristic UUID.
         */
        const char* makeCharacteristicUUID();
        int hexStringToInt(char* str, int endIdx);
        bool insertIncrementedUUID(char* str, int endIdx, int number);
    };

}}}