/**
 * A* �㷨
 * author: zhangpanyi@live.com
 * https://github.com/zhangpanyi/a-star.git
 */

#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <vector>
#include <memory>
#include <cstdint>
#include <functional>

class BlockAllocator;

class AStar
{
public:
    /**
     * ��ά����
     */
    struct Vec2
    {
        uint16_t x;
        uint16_t y;

        Vec2() : x(0) , y(0)
        {
        }

        Vec2(uint16_t x1, uint16_t y1) : x(x1), y(y1)
        {
        }

        void reset(uint16_t x1, uint16_t y1)
        {
            x = x1;
            y = y1;
        }

        bool operator== (const Vec2 &other) const
        {
            return x == other.x && y == other.y;
        }
    };

    typedef std::function<bool(const Vec2&)> Callback;

    /**
     * ��������
     */
    struct Params
    {
        bool        corner;     // ����ս�
        uint16_t    height;     // ��ͼ�߶�
        uint16_t    width;      // ��ͼ���
        Vec2        start;      // �������
        Vec2        end;        // �յ�����
        Callback    can_pass;   // �Ƿ��ͨ��

        Params() : height(0), width(0), corner(false)
        {
        }
    };

private:
    /**
     * ·���ڵ�״̬
     */
    enum NodeState
    {
        NOTEXIST,               // ������
        IN_OPENLIST,            // �ڿ����б�
        IN_CLOSEDLIST           // �ڹر��б�
    };

    /**
     * ·���ڵ�
     */
    struct Node
    {
        uint16_t    g;          // ��������
        uint16_t    h;          // ���յ����
        Vec2        pos;        // �ڵ�λ��
        NodeState   state;      // �ڵ�״̬
        Node*       parent;     // ���ڵ�

        /**
         * ����fֵ
         */
        int f() const
        {
            return g + h;
        }

        inline Node(const Vec2 &pos)
            : g(0), h(0), pos(pos), parent(nullptr), state(NOTEXIST)
        {
        }
    };

public:
    AStar(BlockAllocator *allocator);

    ~AStar();

public:
    /**
     * ��ȡֱ�й�ֵ
     */
    int get_step_value() const;

    /**
     * ��ȡ�սǹ�ֵ
     */
    int get_oblique_value() const;

    /**
     * ����ֱ�й�ֵ
     */
    void set_step_value(int value);

    /**
     * ��ȡ�սǹ�ֵ
     */
    void set_oblique_value(int value);

    /**
     * ִ��Ѱ·����
     */
    std::vector<Vec2> find(const Params &param);

private:
    /**
     * �������
     */
    void clear();

    /**
     * ��ʼ������
     */
    void init(const Params &param);

    /**
     * �����Ƿ���Ч
     */
    bool is_vlid_params(const Params &param);

private:
    /**
     * ���������
     */
    void percolate_up(size_t hole);

    /**
     * ��ȡ�ڵ�����
     */
    bool get_node_index(Node *node, size_t *index);

    /**
     * ����Gֵ
     */
    uint16_t calcul_g_value(Node *parent, const Vec2 &current);

    /**
     * ����Fֵ
     */
    uint16_t calcul_h_value(const Vec2 &current, const Vec2 &end);

    /**
     * �ڵ��Ƿ�����ڿ����б�
     */
    bool in_open_list(const Vec2 &pos, Node *&out_node);

    /**
     * �ڵ��Ƿ�����ڹر��б�
     */
    bool in_closed_list(const Vec2 &pos);

    /**
     * �Ƿ��ͨ��
     */
    bool can_pass(const Vec2 &pos);

    /**
     * ��ǰ���Ƿ�ɵ���Ŀ���
     */
    bool can_pass(const Vec2 &current, const Vec2 &destination, bool allow_corner);

    /**
     * ���Ҹ�����ͨ���Ľڵ�
     */
    void find_can_pass_nodes(const Vec2 &current, bool allow_corner, std::vector<Vec2> *out_lists);

    /**
     * �����ҵ��ڵ�����
     */
    void handle_found_node(Node *current, Node *destination);

    /**
     * ����δ�ҵ��ڵ�����
     */
    void handle_not_found_node(Node *current, Node *destination, const Vec2 &end);

private:
    int                     step_val_;
    int                     oblique_val_;
    std::vector<Node*>      mapping_;
    uint16_t                height_;
    uint16_t                width_;
    Callback                can_pass_;
    std::vector<Node*>      open_list_;
    BlockAllocator*         allocator_;
};

#endif