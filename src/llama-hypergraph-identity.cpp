#include "llama-hypergraph-identity.h"

#include "ggml-backend.h"
#include "ggml.h"
#include "llama-impl.h"

#include <algorithm>
#include <cassert>

namespace regimazone {

hypergraph_identity_framework::hypergraph_identity_framework()
    : next_node_id(0), next_edge_id(0) {
    current_org_context.awareness_level = org_awareness_level::BASIC;
}

hypergraph_identity_framework::~hypergraph_identity_framework() {
    identity_nodes.clear();
    hyperedges.clear();
}

// Identity management
int64_t hypergraph_identity_framework::add_identity_node(identity_type type, const std::string & name) {
    int64_t id = next_node_id++;
    auto node = std::make_unique<hypergraph_identity_node>(id, type, name);
    identity_nodes[id] = std::move(node);
    return id;
}

bool hypergraph_identity_framework::remove_identity_node(int64_t id) {
    auto it = identity_nodes.find(id);
    if (it == identity_nodes.end()) {
        return false;
    }
    
    // Remove any hyperedges connected to this node
    std::vector<int64_t> edges_to_remove;
    for (const auto & edge_pair : hyperedges) {
        const auto & node_ids = edge_pair.second->node_ids;
        if (std::find(node_ids.begin(), node_ids.end(), id) != node_ids.end()) {
            edges_to_remove.push_back(edge_pair.first);
        }
    }
    
    for (int64_t edge_id : edges_to_remove) {
        hyperedges.erase(edge_id);
    }
    
    identity_nodes.erase(it);
    return true;
}

hypergraph_identity_node * hypergraph_identity_framework::get_identity_node(int64_t id) {
    auto it = identity_nodes.find(id);
    if (it != identity_nodes.end()) {
        return it->second.get();
    }
    return nullptr;
}

// HyperEdge management
int64_t hypergraph_identity_framework::add_hyperedge(
        const std::vector<int64_t> & node_ids,
        const std::string & relation_type) {
    
    // Validate that all nodes exist
    for (int64_t node_id : node_ids) {
        if (identity_nodes.find(node_id) == identity_nodes.end()) {
            return -1;
        }
    }
    
    int64_t id = next_edge_id++;
    auto edge = std::make_unique<hypergraph_identity_edge>(id, node_ids, relation_type);
    hyperedges[id] = std::move(edge);
    return id;
}

bool hypergraph_identity_framework::remove_hyperedge(int64_t id) {
    auto it = hyperedges.find(id);
    if (it == hyperedges.end()) {
        return false;
    }
    hyperedges.erase(it);
    return true;
}

hypergraph_identity_edge * hypergraph_identity_framework::get_hyperedge(int64_t id) {
    auto it = hyperedges.find(id);
    if (it != hyperedges.end()) {
        return it->second.get();
    }
    return nullptr;
}

// Organization context management
void hypergraph_identity_framework::set_org_context(const org_context & ctx) {
    current_org_context = ctx;
}

org_context hypergraph_identity_framework::get_org_context() const {
    return current_org_context;
}

// Query operations
std::vector<int64_t> hypergraph_identity_framework::query_identities_by_type(identity_type type) {
    std::vector<int64_t> result;
    for (const auto & pair : identity_nodes) {
        if (pair.second->type == type) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int64_t> hypergraph_identity_framework::query_connected_identities(int64_t node_id) {
    std::vector<int64_t> result;
    
    // Find all hyperedges containing this node
    for (const auto & edge_pair : hyperedges) {
        const auto & node_ids = edge_pair.second->node_ids;
        if (std::find(node_ids.begin(), node_ids.end(), node_id) != node_ids.end()) {
            // Add all other nodes from this hyperedge
            for (int64_t connected_id : node_ids) {
                if (connected_id != node_id &&
                    std::find(result.begin(), result.end(), connected_id) == result.end()) {
                    result.push_back(connected_id);
                }
            }
        }
    }
    
    return result;
}

std::vector<int64_t> hypergraph_identity_framework::query_by_relation_type(const std::string & relation_type) {
    std::vector<int64_t> result;
    for (const auto & edge_pair : hyperedges) {
        if (edge_pair.second->relation_type == relation_type) {
            // Return all nodes in edges with this relation type
            for (int64_t node_id : edge_pair.second->node_ids) {
                if (std::find(result.begin(), result.end(), node_id) == result.end()) {
                    result.push_back(node_id);
                }
            }
        }
    }
    return result;
}

// Org-aware operations
bool hypergraph_identity_framework::validate_org_aware_access(int64_t node_id) const {
    // Basic validation based on org awareness level
    auto it = identity_nodes.find(node_id);
    if (it == identity_nodes.end()) {
        return false;
    }
    
    // If org awareness is NONE, allow all access
    if (current_org_context.awareness_level == org_awareness_level::NONE) {
        return true;
    }
    
    // For higher levels, check if node has org context attribute
    const auto & node = it->second;
    auto attr_it = node->attributes.find("org_context");
    if (attr_it == node->attributes.end()) {
        return current_org_context.awareness_level == org_awareness_level::BASIC;
    }
    
    // Validate org context matches
    return attr_it->second == current_org_context.org_name;
}

std::vector<int64_t> hypergraph_identity_framework::get_org_scoped_identities() const {
    std::vector<int64_t> result;
    
    for (const auto & pair : identity_nodes) {
        if (validate_org_aware_access(pair.first)) {
            result.push_back(pair.first);
        }
    }
    
    return result;
}

// Graph tensor integration
// Note: These functions require linking with ggml library
ggml_tensor * hypergraph_identity_framework::build_identity_tensor(
        ggml_context * ctx,
        int64_t node_id) {
    
    auto it = identity_nodes.find(node_id);
    if (it == identity_nodes.end() || !ctx) {
        return nullptr;
    }

    // Tensor creation is available only when linked with ggml
    // This is a placeholder implementation that returns nullptr
    // In a full build with ggml, this would create the actual tensor
    return nullptr;
}

ggml_tensor * hypergraph_identity_framework::build_hypergraph_adjacency_tensor(ggml_context * ctx) {
    GGML_UNUSED(ctx);

    // Tensor creation is available only when linked with ggml
    // This is a placeholder implementation that returns nullptr
    // In a full build with ggml, this would create the actual adjacency matrix tensor
    return nullptr;
}

// Statistics
size_t hypergraph_identity_framework::get_identity_count() const {
    return identity_nodes.size();
}

size_t hypergraph_identity_framework::get_hyperedge_count() const {
    return hyperedges.size();
}

} // namespace regimazone
