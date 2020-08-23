#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>

class Node
{
public:
    Node(int x, int y)
    {
        this->position[0] = x;
        this->position[1] = y;
        this->path_length = std::numeric_limits<double>::infinity();
        this->global_score = std::numeric_limits<double>::infinity();
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

    double global_score;

    Eigen::Vector2f position;

    Node *parent;

    std::vector<Node **> neighbours;

    bool visited;
};

void solve(Node &start, Node &end, std::map<std::string, Node *> &map)
{
    std::list<Node *> set_of_nodes;

    start.path_length = 0;
    start.global_score = start.dist(end);

    set_of_nodes.push_back(&start);

    while (!set_of_nodes.empty())
    {
        // usleep(20000);
        set_of_nodes.sort([](const Node *a, const Node *b) { return a->global_score < b->global_score; });

        Node *curr_node = set_of_nodes.front();

        if (curr_node->visited)
        {
            set_of_nodes.pop_front();
            continue;
        }

        if (curr_node == &end)
        {
            return;
        }

        std::vector<int> offset = {1, 0, -1, 0, 1, -1, -1, 1};

        // find neighbours
        for (int i = 0; i < 8; ++i)
        {
            std::stringstream _n;
            int row = curr_node->position[0] + offset[i];
            int col = curr_node->position[1] + offset[(i == 0) ? 7 : i - 1];
            _n << row << "," << col;

            if (map.count(_n.str()) == 1)
            {
                curr_node->neighbours.push_back(&map[_n.str()]);
            }
        }

        for (auto n : curr_node->neighbours)
        {
            float tentative_score = curr_node->path_length + (*n)->dist(*curr_node);
            if (tentative_score < (*n)->path_length)
            {
                // update neighbour
                (*n)->parent = curr_node;
                (*n)->path_length = tentative_score;
                (*n)->global_score = tentative_score + (*n)->dist(end);
            }
            if (std::count(set_of_nodes.begin(), set_of_nodes.end(), (*n)) == 0)
            {
                set_of_nodes.push_back(*n);
            }
        }
        curr_node->visited = true;
    }
}

int main(int argc, char **argv)
{
    cv::Mat map_img = cv::imread("/Users/charliehe/Projects/astar/map_view.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    std::string _start_pt{"385,255"};
    std::string _end_pt{"393,105"};

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
            map_img.at<uint8_t>(_n->position[0], _n->position[1]) = 192;
            _n = _n->parent;
        } while (_n->parent != NULL);

        cv::imshow("Win", map_img);
        cv::waitKey(0);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}