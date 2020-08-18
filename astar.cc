#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>

class Node
{
public:
    Node(int x, int y)
    {
        this->position[0] = x;
        this->position[1] = y;
        this->path_length = std::numeric_limits<double>::infinity();
        this->g_score = std::numeric_limits<double>::infinity();
        this->parent = NULL;
        this->visited = false;
    }

    ~Node()
    {
        return;
    }

    void update()
    {
    }

    float dist(Node &_node)
    {
        return (this->position - _node.position).norm();
    }

    double path_length;

    double g_score;

    Eigen::Vector2f position;

    Node *parent;

    std::vector<Node **> neighbours;

    bool visited;
};

void solve(Node &start, Node &end, std::map<std::string, Node *> &map)
{
    std::list<Node *> set_of_nodes;

    start.path_length = 0;
    start.g_score = start.dist(end);

    set_of_nodes.push_back(&start);

    while (!set_of_nodes.empty())
    {
        // std::cout << set_of_nodes.size() << std::endl;
        set_of_nodes.sort([](const Node *a, const Node *b) { return a->g_score < b->g_score; });

        Node *curr_node = set_of_nodes.front();

        // std::cout << curr_node->g_score << std::endl;

        // find neighbours
        for (int i = 0; i < 4; ++i)
        {
            std::stringstream _n;
            int row = curr_node->position[0] + 5 - abs(5 * (i - 2));
            int col = curr_node->position[1] + 5 - abs(5 * (i - 1));
            _n << row << "," << col;

            if (map.count(_n.str()) == 1)
            {
                curr_node->neighbours.push_back(&map[_n.str()]);
            }
        }

        if (curr_node == &end)
        {
            break;
        }

        for (auto n : curr_node->neighbours)
        {
            if (curr_node->path_length + 1 < (*n)->path_length)
            {
                // update neighbour
                (*n)->visited = false;
                (*n)->parent = curr_node;
                (*n)->path_length = curr_node->path_length + 1;
                (*n)->g_score = (*n)->path_length + (*n)->dist(end);
            }
            if (std::count(set_of_nodes.begin(), set_of_nodes.end(), (*n)) == 0)
            {
                set_of_nodes.push_back(*n);
            }
        }
        curr_node->visited = true;
        set_of_nodes.pop_front();
        // sleep(1);
    }
    // std::cout << end.parent->position << std::endl;
}


int main(int argc, char **argv)
{
    cv::Mat map_img = cv::imread("/home/hcrd/Projects/astar/map_view.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    std::string _start_pt{"400,325"};
    std::string _end_pt{"350,450"};

    std::map<std::string, Node *> map_graph;
    for (size_t row = 0; row < map_img.rows; ++row)
    {
        for (size_t col = 0; col < map_img.cols; ++col)
        {
            if (map_img.at<uint8_t>(row, col) == 0)
            {
                std::stringstream _point;
                _point << row << "," << col;
                Node *new_node = new Node(row, col);
                map_graph.emplace(_point.str(), new_node);
            }
        }
    }

    try
    {
        Node *start_node = map_graph[_start_pt];
        Node *end_node = map_graph[_end_pt];
        solve(*start_node, *end_node, map_graph);

        Node *_n = end_node;
        do
        {
            std::cout << _n->position << std::endl;
            _n = _n->parent;
        }while(_n->parent != NULL);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}