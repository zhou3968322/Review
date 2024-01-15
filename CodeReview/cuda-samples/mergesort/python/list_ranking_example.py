import random

node_next_dict = {8: 0, 7:8, 5:7, 3:5, 6:3, 2:6, 4:2}
node_prev_dict = {0:8, 8:7, 7:5, 5:3, 3:6, 6:2, 2:4}
nodes = [4, 6, 5, 2, 7, 3, 8, 0]
distance_dict  = {}
for node in nodes:
    distance_dict[node] = 0



# just compute depth not weight
def list_ranking(node_list, node_next, node_prev):
    assert isinstance(node_next, dict)
    assert isinstance(node_prev, dict)
    n = len(node_list)
    if n <= 3:
        for i, node in enumerate(node_list):
            distance_dict[node] = 2 - i
        return
    random_bools = [random.choice([True, False]) for _ in range(n)]
    # True head, False tail
    # all node follow tail
    tail_follow_node_i_list = []
    for i, node in enumerate(node_list):
        if not random_bools[i] and node in node_next:
            tail_follow_node_i_list.append(node_next[node])
    delted_node_dict = {}
    new_nodes = []
    org_node_prev = node_prev.copy()
    for i, node in enumerate(node_list):
        if random_bools[i] and node in tail_follow_node_i_list and node in node_next and node in node_prev:
            prev = node_prev.pop(node)
            next = node_next.pop(node)
            node_next[prev] = next
            node_prev[next] = prev
            delted_node_dict[node] = [next, prev]
        else:
            new_nodes.append(node)
    list_ranking(new_nodes, node_next, node_prev)

    for node in delted_node_dict:
        next, prev = delted_node_dict[node]
        distance_dict[node] = distance_dict[next] + 1
        distance_dict[prev] = distance_dict[node] + 1
        while prev in org_node_prev:
            distance_dict[org_node_prev[prev]] = distance_dict[prev] + 1
            prev = org_node_prev[prev]

list_ranking(nodes, node_next=node_next_dict, node_prev=node_prev_dict)

for node in nodes:
    print("distance of node:{} is {}".format(node, distance_dict[node]))



