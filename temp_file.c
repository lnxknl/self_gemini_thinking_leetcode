#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Struct definitions
typedef struct {
    char license_plate[20];
    int owner_id;
    bool is_stolen;
    int outstanding_tickets;
} VehicleRecord;

typedef struct {
    int id;
    char name[50];
    char driver_license_number[20];
    bool is_license_suspended;
    bool outstanding_arrest_warrants;
} IndividualRecord;

// Function to find a vehicle record by license plate
VehicleRecord* findVehicle(const char* license_plate, VehicleRecord* vehicle_records[], int num_vehicles) {
    for (int i = 0; i < num_vehicles; i++) {
        if (strcmp(vehicle_records[i]->license_plate, license_plate) == 0) {
            return vehicle_records[i];
        }
    }
    return NULL;
}

// Function to find an individual record by ID
IndividualRecord* findIndividual(int id, IndividualRecord* individual_records[], int num_individuals) {
    for (int i = 0; i < num_individuals; i++) {
        if (individual_records[i]->id == id) {
            return individual_records[i];
        }
    }
    return NULL;
}

// Main function to determine the action
const char* determineAction(const char* license_plate, VehicleRecord* vehicle_records[], int num_vehicles, IndividualRecord* individual_records[], int num_individuals) {
    VehicleRecord* vehicle = findVehicle(license_plate, vehicle_records, num_vehicles);

    if (vehicle == NULL) {
        return "Error: Vehicle not found in database.";
    }

    if (vehicle->is_stolen) {
        return "Arrest Driver, Impound Vehicle";
    }

    IndividualRecord* owner = findIndividual(vehicle->owner_id, individual_records, num_individuals);

    if (owner == NULL) {
        return "Error: Owner information not found.";
    }

    if (owner->outstanding_arrest_warrants) {
        return "Arrest Driver";
    }

    if (owner->is_license_suspended) {
        return "Issue Citation: Driving with Suspended License";
    }

    if (vehicle->outstanding_tickets > 5) {
        return "Issue Citation: Excessive Unpaid Tickets";
    } else if (vehicle->outstanding_tickets > 0) {
        return "Warn Driver about Unpaid Tickets";
    }

    return "Routine Stop, No Action Required";
}

int main() {
    // Test Input
    VehicleRecord vehicle1 = {"ABC-123", 101, false, 2};
    VehicleRecord vehicle2 = {"XYZ-789", 102, true, 0};
    VehicleRecord vehicle3 = {"LMN-456", 103, false, 7};

    IndividualRecord individual1 = {101, "John Doe", "DL12345", false, false};
    IndividualRecord individual2 = {102, "Jane Smith", "DL67890", false, true};
    IndividualRecord individual3 = {103, "Peter Jones", "DL11111", true, false};

    VehicleRecord* vehicle_records[] = {&vehicle1, &vehicle2, &vehicle3};
    IndividualRecord* individual_records[] = {&individual1, &individual2, &individual3};
    int num_vehicles = 3;
    int num_individuals = 3;

    // Test Cases
    printf("License Plate ABC-123: %s\n", determineAction("ABC-123", vehicle_records, num_vehicles, individual_records, num_individuals)); // Expected: Warn Driver about Unpaid Tickets
    printf("License Plate XYZ-789: %s\n", determineAction("XYZ-789", vehicle_records, num_vehicles, individual_records, num_individuals)); // Expected: Arrest Driver, Impound Vehicle
    printf("License Plate LMN-456: %s\n", determineAction("LMN-456", vehicle_records, num_vehicles, individual_records, num_individuals)); // Expected: Issue Citation: Excessive Unpaid Tickets

    // Add a case for suspended license
    VehicleRecord vehicle4 = {"PQR-999", 103, false, 0};
    VehicleRecord* vehicle_records2[] = {&vehicle1, &vehicle2, &vehicle3, &vehicle4};
    printf("License Plate PQR-999: %s\n", determineAction("PQR-999", vehicle_records2, 4, individual_records, num_individuals)); // Expected: Issue Citation: Driving with Suspended License

    // Add a case for arrest warrant
    VehicleRecord vehicle5 = {"STU-000", 102, false, 1};
    VehicleRecord* vehicle_records3[] = {&vehicle1, &vehicle2, &vehicle3, &vehicle4, &vehicle5};
    printf("License Plate STU-000: %s\n", determineAction("STU-000", vehicle_records3, 5, individual_records, num_individuals)); // Expected: Arrest Driver

    // Add a case for routine stop
    VehicleRecord vehicle6 = {"VVV-111", 104, false, 0};
    IndividualRecord individual4 = {104, "Sarah Blue", "DL22222", false, false};
    VehicleRecord* vehicle_records4[] = {&vehicle1, &vehicle2, &vehicle3, &vehicle4, &vehicle5, &vehicle6};
    IndividualRecord* individual_records2[] = {&individual1, &individual2, &individual3, &individual4};
    printf("License Plate VVV-111: %s\n", determineAction("VVV-111", vehicle_records4, 6, individual_records2, 4)); // Expected: Routine Stop, No Action Required

    return 0;
}
