#include "llama-hypergraph-identity.h"

#include <cassert>
#include <iostream>
#include <vector>

// Simple unit tests for the Regimazone HyperGraphQL OrgAware Identity Framework

void test_basic_node_creation() {
    std::cout << "Testing basic node creation... ";
    regimazone::hypergraph_identity_framework framework;
    
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "test_node");
    assert(node1 == 0);
    assert(framework.get_identity_count() == 1);
    
    auto * node = framework.get_identity_node(node1);
    assert(node != nullptr);
    assert(node->name == "test_node");
    assert(node->type == regimazone::identity_type::NODE_IDENTITY);
    
    std::cout << "PASSED\n";
}

void test_node_removal() {
    std::cout << "Testing node removal... ";
    regimazone::hypergraph_identity_framework framework;
    
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node1");
    int64_t node2 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node2");
    
    assert(framework.get_identity_count() == 2);
    
    bool removed = framework.remove_identity_node(node1);
    assert(removed);
    assert(framework.get_identity_count() == 1);
    
    auto * node = framework.get_identity_node(node1);
    assert(node == nullptr);
    
    std::cout << "PASSED\n";
}

void test_hyperedge_creation() {
    std::cout << "Testing hyperedge creation... ";
    regimazone::hypergraph_identity_framework framework;
    
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node1");
    int64_t node2 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node2");
    int64_t node3 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node3");
    
    int64_t edge = framework.add_hyperedge({node1, node2, node3}, "test_relation");
    assert(edge == 0);
    assert(framework.get_hyperedge_count() == 1);
    
    auto * edge_obj = framework.get_hyperedge(edge);
    assert(edge_obj != nullptr);
    assert(edge_obj->node_ids.size() == 3);
    assert(edge_obj->relation_type == "test_relation");
    
    std::cout << "PASSED\n";
}

void test_query_by_type() {
    std::cout << "Testing query by type... ";
    regimazone::hypergraph_identity_framework framework;
    
    framework.add_identity_node(regimazone::identity_type::NODE_IDENTITY, "node1");
    framework.add_identity_node(regimazone::identity_type::NODE_IDENTITY, "node2");
    framework.add_identity_node(regimazone::identity_type::ORG_CONTEXT_IDENTITY, "org1");
    
    auto nodes = framework.query_identities_by_type(regimazone::identity_type::NODE_IDENTITY);
    assert(nodes.size() == 2);
    
    auto orgs = framework.query_identities_by_type(regimazone::identity_type::ORG_CONTEXT_IDENTITY);
    assert(orgs.size() == 1);
    
    std::cout << "PASSED\n";
}

void test_query_connected() {
    std::cout << "Testing query connected identities... ";
    regimazone::hypergraph_identity_framework framework;
    
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node1");
    int64_t node2 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node2");
    int64_t node3 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node3");
    
    framework.add_hyperedge({node1, node2}, "edge1");
    framework.add_hyperedge({node1, node3}, "edge2");
    
    auto connected = framework.query_connected_identities(node1);
    assert(connected.size() == 2); // node2 and node3
    
    connected = framework.query_connected_identities(node2);
    assert(connected.size() == 1); // only node1
    
    std::cout << "PASSED\n";
}

void test_query_by_relation() {
    std::cout << "Testing query by relation type... ";
    regimazone::hypergraph_identity_framework framework;
    
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node1");
    int64_t node2 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node2");
    int64_t node3 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node3");
    
    framework.add_hyperedge({node1, node2}, "collaborates");
    framework.add_hyperedge({node2, node3}, "reports_to");
    
    auto collaborators = framework.query_by_relation_type("collaborates");
    assert(collaborators.size() == 2); // node1 and node2
    
    auto reports = framework.query_by_relation_type("reports_to");
    assert(reports.size() == 2); // node2 and node3
    
    std::cout << "PASSED\n";
}

void test_org_context() {
    std::cout << "Testing org context... ";
    regimazone::hypergraph_identity_framework framework;
    
    regimazone::org_context ctx("test_org", regimazone::org_awareness_level::CONTEXTUAL);
    framework.set_org_context(ctx);
    
    auto retrieved_ctx = framework.get_org_context();
    assert(retrieved_ctx.org_name == "test_org");
    assert(retrieved_ctx.awareness_level == regimazone::org_awareness_level::CONTEXTUAL);
    
    std::cout << "PASSED\n";
}

void test_cascade_delete() {
    std::cout << "Testing cascade delete... ";
    regimazone::hypergraph_identity_framework framework;
    
    int64_t node1 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node1");
    int64_t node2 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node2");
    int64_t node3 = framework.add_identity_node(
        regimazone::identity_type::NODE_IDENTITY, "node3");
    
    framework.add_hyperedge({node1, node2}, "edge1");
    framework.add_hyperedge({node1, node3}, "edge2");
    framework.add_hyperedge({node2, node3}, "edge3");
    
    assert(framework.get_hyperedge_count() == 3);
    
    // Remove node1, should also remove edge1 and edge2
    framework.remove_identity_node(node1);
    assert(framework.get_hyperedge_count() == 1); // only edge3 remains
    
    std::cout << "PASSED\n";
}

int main() {
    std::cout << "=== Regimazone HyperGraphQL OrgAware Identity Framework Tests ===\n\n";
    
    test_basic_node_creation();
    test_node_removal();
    test_hyperedge_creation();
    test_query_by_type();
    test_query_connected();
    test_query_by_relation();
    test_org_context();
    test_cascade_delete();
    
    std::cout << "\n=== All Tests PASSED ===\n";
    
    return 0;
}
