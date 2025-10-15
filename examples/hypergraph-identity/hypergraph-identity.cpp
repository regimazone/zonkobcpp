#include "llama-hypergraph-identity.h"

#include <iostream>
#include <cassert>

// Simple demonstration of the Regimazone HyperGraphQL OrgAware Identity Framework

int main() {
    std::cout << "=== Regimazone HyperGraphQL OrgAware Identity Framework Demo ===\n\n";
    
    // Create the framework instance
    regimazone::hypergraph_identity_framework framework;
    
    // Set organizational context
    regimazone::org_context org_ctx("regimazone", regimazone::org_awareness_level::CONTEXTUAL);
    framework.set_org_context(org_ctx);
    
    std::cout << "1. Creating identity nodes...\n";
    
    // Add some identity nodes
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "user_alice");
    int64_t node2 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "user_bob");
    int64_t node3 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "user_charlie");
    int64_t org_node = framework.add_identity_node(
        regimazone::identity_type::ORG_CONTEXT_IDENTITY, "org_regimazone");
    
    std::cout << "   Created nodes: " << node1 << ", " << node2 << ", " 
              << node3 << ", " << org_node << "\n\n";
    
    std::cout << "2. Adding hyperedges...\n";
    
    // Create hyperedges connecting the nodes
    int64_t edge1 = framework.add_hyperedge({node1, node2}, "collaborates_with");
    int64_t edge2 = framework.add_hyperedge({node2, node3}, "reports_to");
    int64_t edge3 = framework.add_hyperedge({node1, node2, node3, org_node}, "member_of");
    
    std::cout << "   Created hyperedges: " << edge1 << ", " << edge2 << ", " << edge3 << "\n\n";
    
    std::cout << "3. Querying the hypergraph...\n";
    
    // Query identities by type
    auto node_identities = framework.query_identities_by_type(regimazone::identity_type::NODE_IDENTITY);
    std::cout << "   Node identities count: " << node_identities.size() << "\n";
    
    auto org_identities = framework.query_identities_by_type(regimazone::identity_type::ORG_CONTEXT_IDENTITY);
    std::cout << "   Org context identities count: " << org_identities.size() << "\n";
    
    // Query connected identities
    auto connected_to_alice = framework.query_connected_identities(node1);
    std::cout << "   Identities connected to Alice: " << connected_to_alice.size() << "\n";
    
    // Query by relation type
    auto collaborators = framework.query_by_relation_type("collaborates_with");
    std::cout << "   Collaborators: " << collaborators.size() << " identities\n\n";
    
    std::cout << "4. Org-aware operations...\n";
    
    // Check org-aware access
    bool access_allowed = framework.validate_org_aware_access(node1);
    std::cout << "   Access to Alice (node " << node1 << "): " 
              << (access_allowed ? "ALLOWED" : "DENIED") << "\n";
    
    // Get org-scoped identities
    auto org_scoped = framework.get_org_scoped_identities();
    std::cout << "   Org-scoped identities: " << org_scoped.size() << "\n\n";
    
    std::cout << "5. Statistics...\n";
    std::cout << "   Total identity nodes: " << framework.get_identity_count() << "\n";
    std::cout << "   Total hyperedges: " << framework.get_hyperedge_count() << "\n\n";
    
    std::cout << "6. Testing node removal...\n";
    bool removed = framework.remove_identity_node(node3);
    std::cout << "   Removed node " << node3 << ": " << (removed ? "SUCCESS" : "FAILED") << "\n";
    std::cout << "   Identity count after removal: " << framework.get_identity_count() << "\n";
    std::cout << "   Hyperedge count after removal: " << framework.get_hyperedge_count() 
              << " (edges containing removed node are also removed)\n\n";
    
    std::cout << "=== Demo Complete ===\n";
    
    return 0;
}
