#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

// Forward declarations
struct ggml_tensor;
struct ggml_context;

// Regimazone HyperGraphQL OrgAware Identity Framework
//
// This framework provides identity-aware graph query capabilities with
// organizational context tracking for hypergraph structures.

namespace regimazone {

// Identity types for org-aware graph operations
enum class identity_type {
    NODE_IDENTITY,
    EDGE_IDENTITY,
    HYPEREDGE_IDENTITY,
    ORG_CONTEXT_IDENTITY,
};

// Organizational awareness levels
enum class org_awareness_level {
    NONE,
    BASIC,
    CONTEXTUAL,
    HIERARCHICAL,
};

// HyperGraph identity node
struct hypergraph_identity_node {
    int64_t id;
    identity_type type;
    std::string name;
    std::unordered_map<std::string, std::string> attributes;
    
    hypergraph_identity_node() : id(-1), type(identity_type::NODE_IDENTITY) {}
    hypergraph_identity_node(int64_t id, identity_type type, const std::string & name)
        : id(id), type(type), name(name) {}
};

// HyperGraph edge connecting multiple identities
struct hypergraph_identity_edge {
    int64_t id;
    std::vector<int64_t> node_ids;
    std::string relation_type;
    
    hypergraph_identity_edge() : id(-1) {}
    hypergraph_identity_edge(int64_t id, const std::vector<int64_t> & nodes, const std::string & rel)
        : id(id), node_ids(nodes), relation_type(rel) {}
};

// Organizational context for identity tracking
struct org_context {
    std::string org_name;
    org_awareness_level awareness_level;
    std::unordered_map<std::string, std::string> context_data;
    
    org_context() : awareness_level(org_awareness_level::NONE) {}
    org_context(const std::string & name, org_awareness_level level)
        : org_name(name), awareness_level(level) {}
};

// Main hypergraph identity framework class
class hypergraph_identity_framework {
public:
    hypergraph_identity_framework();
    ~hypergraph_identity_framework();
    
    // Identity management
    int64_t add_identity_node(identity_type type, const std::string & name);
    bool remove_identity_node(int64_t id);
    hypergraph_identity_node * get_identity_node(int64_t id);
    
    // HyperEdge management
    int64_t add_hyperedge(const std::vector<int64_t> & node_ids, const std::string & relation_type);
    bool remove_hyperedge(int64_t id);
    hypergraph_identity_edge * get_hyperedge(int64_t id);
    
    // Organization context management
    void set_org_context(const org_context & ctx);
    org_context get_org_context() const;
    
    // Query operations
    std::vector<int64_t> query_identities_by_type(identity_type type);
    std::vector<int64_t> query_connected_identities(int64_t node_id);
    std::vector<int64_t> query_by_relation_type(const std::string & relation_type);
    
    // Org-aware operations
    bool validate_org_aware_access(int64_t node_id) const;
    std::vector<int64_t> get_org_scoped_identities() const;
    
    // Graph tensor integration
    ggml_tensor * build_identity_tensor(ggml_context * ctx, int64_t node_id);
    ggml_tensor * build_hypergraph_adjacency_tensor(ggml_context * ctx);
    
    // Statistics
    size_t get_identity_count() const;
    size_t get_hyperedge_count() const;
    
private:
    std::unordered_map<int64_t, std::unique_ptr<hypergraph_identity_node>> identity_nodes;
    std::unordered_map<int64_t, std::unique_ptr<hypergraph_identity_edge>> hyperedges;
    org_context current_org_context;
    
    int64_t next_node_id;
    int64_t next_edge_id;
};

} // namespace regimazone
