add_executable(llama-hypergraph-identity hypergraph-identity.cpp)
target_include_directories(llama-hypergraph-identity PRIVATE ../../src)
target_link_libraries(llama-hypergraph-identity PRIVATE common)
