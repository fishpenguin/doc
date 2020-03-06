#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Base struct of all fields
struct Field {
    std::string field_name;
    std::string field_type;
    float boost;
    std::string extram_params;
}

// DistanceMetric
enum class DistanceMetric {
    L2 = 1,        // Euclidean Distance
    IP = 2,        // Cosine Similarity
    HAMMING = 3,   // Hamming Distance
    JACCARD = 4,   // Jaccard Distance
    TANIMOTO = 5,  // Tanimoto Distance
};

// vector field
struct VectorField : Field {
    DistanceMetric distance_metric;
    uint64_t dimension;
    uint64_t index_file_size;
}