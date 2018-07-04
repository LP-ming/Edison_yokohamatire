#include <stdlib.h>								
#include <signal.h>								
#include <stdio.h>								
#include <unistd.h>								
#include <string.h>	
#include <cassert>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <memory.h>
#include <stddef.h>
#include <malloc.h>
#include <assert.h>
#include <iostream>
#include <exception>
#include<sys/time.h>
#include<time.h>
#include<math.h>

#include "astar.h"
#include "blockallocator.h"				

				
/*********************************************

              起点---终点数据解析

*********************************************/
int areaCode1x;
int areaCode1y;
int areaCode2x;
int areaCode2y;
//将提供的4个目标和1个起点值输入
char target_start[8]={'A','0','F','0','A','5','F','5'};
char target_end[8]={'F','5','A','5','A','0','F','0'};

void move_target(int c)
  {
   areaCode1y=target_start[c]-16-48-1;
   areaCode1x=target_start[c+1]-48; 
   areaCode2y=target_end[c]-16-48-1;
   areaCode2x=target_end[c+1]-48;
   printf("起点区域坐标=%c%c\n\n",target_end[c],target_end[c+1]);
   printf("终点区域坐标=%c%c\n\n",target_end[c],target_end[c+1]);
  }

/********************************************	

            搜索函数设定&执行

********************************************/
int target1x;//终点坐标x
int target1y;//终点坐标y
int astar_go1_len;//规划路径长度
void astar_go1()
{
    char maps1[6][6] =
    {
      { 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0 },
      { 0, 1, 1, 0, 1, 0 },
      { 0, 0, 0, 0, 0, 0 },
      { 0, 1, 0, 1, 1, 0 },
      { 0, 0, 0, 0, 0, 0 },
    };

    AStar::Params param1;
    param1.width = 6;
    param1.height = 6;
    param1.corner = false;
    param1.start = AStar::Vec2(areaCode1x,areaCode1y);
    param1.end = AStar::Vec2(areaCode2x, areaCode2y);
    param1.can_pass = [&](const AStar::Vec2 &pos)->bool
    {
     return maps1[pos.y][pos.x] == 0;
    };
    // 执行搜索
    BlockAllocator allocator;
    AStar algorithm(&allocator);
    auto path = algorithm.find(param1);
    // target1x=path[0].x;
    // target1y=path[0].y;

    for(int i=0;i<20;i++)
        {
            if(path[i].x>=0&&path[i].x<=5&&path[i].y>=0&&path[i].y<=5)
            {
                target1x=path[i].x+16+48+1;
                target1y=path[i].y+48;   
                printf("第%d步：%c,%c\n",i,target1x,target1y);
            }
            else
            {
                astar_go1_len=i;
                printf("规划路径一共%d步\n",astar_go1_len);
            }
        }
}

/****************以下主程序*****************/
int main()														
{		 												
 //遍历4个起点，终点，规划出4个路径
 for(int i=0;i<4;i++)
  {	   
    move_target(i*2);
    astar_go1();
    sleep(3);
  }		
   return 1;   								
}




/********************************************						

	    astar.cpp代码                     														

********************************************/
static const int kStepValue = 10;
static const int kObliqueValue = 14;

AStar::AStar(BlockAllocator *allocator)
    : width_(0)
    , height_(0)
    , allocator_(allocator)
    , step_val_(kStepValue)
    , oblique_val_(kObliqueValue)
{
    assert(allocator_ != nullptr);
}

AStar::~AStar()
{
    clear();
}

// 获取直行估值
int AStar::get_step_value() const
{
    return step_val_;
}

// 获取拐角估值
int AStar::get_oblique_value() const
{
    return oblique_val_;
}

// 设置直行估值
void AStar::set_step_value(int value)
{
    step_val_ = value;
}

// 获取拐角估值
void AStar::set_oblique_value(int value)
{
    oblique_val_ = value;
}

// 清理参数
void AStar::clear()
{
    size_t index = 0;
    const size_t max_size = width_ * height_;
    while (index < max_size)
    {
        allocator_->free(mapping_[index++], sizeof(Node));
        index++;
    }
    open_list_.clear();
    can_pass_ = nullptr;
    width_ = height_ = 0;
}

// 初始化操作
void AStar::init(const Params &param)
{
    width_ = param.width;
    height_ = param.height;
    can_pass_ = param.can_pass;
    if (!mapping_.empty())
    {
        memset(&mapping_[0], 0, sizeof(Node*) * mapping_.size());
    }
    mapping_.resize(width_ * height_, nullptr);
}

// 参数是否有效
bool AStar::is_vlid_params(const AStar::Params &param)
{
    return (param.can_pass != nullptr
            && (param.width > 0 && param.height > 0)
            && (param.end.x >= 0 && param.end.x < param.width)
            && (param.end.y >= 0 && param.end.y < param.height)
            && (param.start.x >= 0 && param.start.x < param.width)
            && (param.start.y >= 0 && param.start.y < param.height)
            );
}

// 获取节点索引
bool AStar::get_node_index(Node *node, size_t *index)
{
    *index = 0;
    const size_t size = open_list_.size();
    while (*index < size)
    {
        if (open_list_[*index]->pos == node->pos)
        {
            return true;
        }
        ++(*index);
    }
    return false;
}

// 二叉堆上滤
void AStar::percolate_up(size_t hole)
{
    size_t parent = 0;
    while (hole > 0)
    {
        parent = (hole - 1) / 2;
        if (open_list_[hole]->f() < open_list_[parent]->f())
        {
            std::swap(open_list_[hole], open_list_[parent]);
            hole = parent;
        }
        else
        {
            return;
        }
    }
}

// 计算G值
inline uint16_t AStar::calcul_g_value(Node *parent, const Vec2 &current)
{
    uint16_t g_value = ((abs(current.y + current.x - parent->pos.y - parent->pos.x)) == 2 ? oblique_val_ : step_val_);
    return g_value += parent->g;
}

// 计算F值
inline uint16_t AStar::calcul_h_value(const Vec2 &current, const Vec2 &end)
{
    unsigned int h_value = abs(end.y + end.x - current.y - current.x);
    return h_value * step_val_;
}

// 节点是否存在于开启列表
inline bool AStar::in_open_list(const Vec2 &pos, Node *&out_node)
{
    out_node = mapping_[pos.y * width_ + pos.x];
    return out_node ? out_node->state == IN_OPENLIST : false;
}

// 节点是否存在于关闭列表
inline bool AStar::in_closed_list(const Vec2 &pos)
{
    Node *node_ptr = mapping_[pos.y * width_ + pos.x];
    return node_ptr ? node_ptr->state == IN_CLOSEDLIST : false;
}

// 是否可到达
bool AStar::can_pass(const Vec2 &pos)
{
    return (pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_) ? can_pass_(pos) : false;
}

// 当前点是否可到达目标点
bool AStar::can_pass(const Vec2 &current, const Vec2 &destination, bool allow_corner)
{
    if (destination.x >= 0 && destination.x < width_ && destination.y >= 0 && destination.y < height_)
    {
        if (in_closed_list(destination))
        {
            return false;
        }

        if (abs(current.y + current.x - destination.y - destination.x) == 1)
        {
            return can_pass_(destination);
        }
        else if (allow_corner)
        {
            return can_pass_(destination) && (can_pass(Vec2(current.x + destination.x - current.x, current.y))
                    && can_pass(Vec2(current.x, current.y + destination.y - current.y)));
        }
    }
    return false;
}

// 查找附近可通过的节点
void AStar::find_can_pass_nodes(const Vec2 &current, bool corner, std::vector<Vec2> *out_lists)
{
    Vec2 destination;
    int row_index = current.y - 1;
    const int max_row = current.y + 1;
    const int max_col = current.x + 1;

    if (row_index < 0)
    {
        row_index = 0;
    }
    
    while (row_index <= max_row)
    {
        int col_index = current.x - 1;

        if (col_index < 0)
        {
            col_index = 0;
        }
    
        while (col_index <= max_col)
        {
            destination.reset(col_index, row_index);
            if (can_pass(current, destination, corner))
            {
                out_lists->push_back(destination);
            }
            ++col_index;
        }
        ++row_index;
    }
}

// 处理找到节点的情况
void AStar::handle_found_node(Node *current, Node *destination)
{
    unsigned int g_value = calcul_g_value(current, destination->pos);
    if (g_value < destination->g)
    {
        destination->g = g_value;
        destination->parent = current;

        size_t index = 0;
        if (get_node_index(destination, &index))
        {
            percolate_up(index);
        }
        else
        {
            assert(false);
        }
    }
}

// 处理未找到节点的情况
void AStar::handle_not_found_node(Node *current, Node *destination, const Vec2 &end)
{
    destination->parent = current;
    destination->h = calcul_h_value(destination->pos, end);
    destination->g = calcul_g_value(current, destination->pos);

    Node *&reference_node = mapping_[destination->pos.y * width_ + destination->pos.x];
    reference_node = destination;
    reference_node->state = IN_OPENLIST;

    open_list_.push_back(destination);
    std::push_heap(open_list_.begin(), open_list_.end(), [](const Node *a, const Node *b)->bool
    {
        return a->f() > b->f();
    });
}

// 执行寻路操作
std::vector<AStar::Vec2> AStar::find(const Params &param)
{
    std::vector<Vec2> paths;
    assert(is_vlid_params(param));
    if (!is_vlid_params(param))
    {
        return paths;
    }

    // 初始化
    init(param);
    std::vector<Vec2> nearby_nodes;
    nearby_nodes.reserve(param.corner ? 8 : 4);

    // 将起点放入开启列表
    Node *start_node = new(allocator_->allocate(sizeof(Node))) Node(param.start);
    open_list_.push_back(start_node);
    Node *&reference_node = mapping_[start_node->pos.y * width_ + start_node->pos.x];
    reference_node = start_node;
    reference_node->state = IN_OPENLIST;

    // 寻路操作
    while (!open_list_.empty())
    {
        // 找出f值最小节点
        Node *current = open_list_.front();
        std::pop_heap(open_list_.begin(), open_list_.end(), [](const Node *a, const Node *b)->bool
        {
            return a->f() > b->f();
        });
        open_list_.pop_back();
        mapping_[current->pos.y * width_ + current->pos.x]->state = IN_CLOSEDLIST;

        // 是否找到终点
        if (current->pos == param.end)
        {
            while (current->parent)
            {
                paths.push_back(current->pos);
                current = current->parent;
            }
            std::reverse(paths.begin(), paths.end());
            goto __end__;
        }

        // 查找周围可通过节点
        nearby_nodes.clear();
        find_can_pass_nodes(current->pos, param.corner, &nearby_nodes);

        // 计算周围节点的估值
        size_t index = 0;
        const size_t size = nearby_nodes.size();
        while (index < size)
        {
            Node *next_node = nullptr;
            if (in_open_list(nearby_nodes[index], next_node))
            {
                handle_found_node(current, next_node);
            }
            else
            {
                next_node = new(allocator_->allocate(sizeof(Node))) Node(nearby_nodes[index]);
                handle_not_found_node(current, next_node, param.end);
            }
            ++index;
        }
    }

__end__:
    clear();
    return paths;
}

/********************************************						

	 blockallocator.cpp代码                     														

********************************************/
struct Chunk
{
    int block_size;
    Block *blocks;
};

struct Block
{
    Block *next;
};

int BlockAllocator::block_sizes_[kBlockSizes] =
{
    16,     // 0
    32,     // 1
    64,     // 2
    96,     // 3
    128,    // 4
    160,    // 5
    192,    // 6
    224,    // 7
    256,    // 8
    320,    // 9
    384,    // 10
    448,    // 11
    512,    // 12
    640,    // 13
};

bool BlockAllocator::s_block_size_lookup_initialized_;

uint8_t BlockAllocator::s_block_size_lookup_[kMaxBlockSize + 1];

BlockAllocator::BlockAllocator()
{
    assert(kBlockSizes < UCHAR_MAX);

    num_chunk_space_ = kChunkArrayIncrement;
    num_chunk_count_ = 0;
    chunks_ = (Chunk *)malloc(num_chunk_space_ * sizeof(Chunk));

    memset(chunks_, 0, num_chunk_space_ * sizeof(Chunk));
    memset(free_lists_, 0, sizeof(free_lists_));

    if (s_block_size_lookup_initialized_ == false)
    {
        int j = 0;
        for (int i = 1; i <= kMaxBlockSize; ++i)
        {
            assert(j < kBlockSizes);
            if (i <= block_sizes_[j])
            {
                s_block_size_lookup_[i] = (uint8_t)j;
            }
            else
            {
                ++j;
                s_block_size_lookup_[i] = (uint8_t)j;
            }
        }
        s_block_size_lookup_initialized_ = true;
    }
}

BlockAllocator::~BlockAllocator()
{
    for (int i = 0; i < num_chunk_count_; ++i)
    {
        ::free(chunks_[i].blocks);
    }
    ::free(chunks_);
}

void* BlockAllocator::allocate(int size)
{
    if (size == 0)
    {
        return nullptr;
    }

    assert(0 < size);

    if (size > kMaxBlockSize)
    {
        return malloc(size);
    }

    int index = s_block_size_lookup_[size];
    assert(0 <= index && index < kBlockSizes);

    if (free_lists_[index])
    {
        Block *block = free_lists_[index];
        free_lists_[index] = block->next;
        return block;
    }
    else
    {
        if (num_chunk_count_ == num_chunk_space_)
        {
            Chunk *oldChunks = chunks_;
            num_chunk_space_ += kChunkArrayIncrement;
            chunks_ = (Chunk *)malloc(num_chunk_space_ * sizeof(Chunk));
            memcpy(chunks_, oldChunks, num_chunk_count_ * sizeof(Chunk));
            memset(chunks_ + num_chunk_count_, 0, kChunkArrayIncrement * sizeof(Chunk));
            ::free(oldChunks);
        }

        Chunk *chunk = chunks_ + num_chunk_count_;
        chunk->blocks = (Block *)malloc(kChunkSize);
#if defined(_DEBUG)
        memset(chunk->blocks, 0xcd, kChunkSize);
#endif
        int block_size = block_sizes_[index];
        chunk->block_size = block_size;
        int block_count = kChunkSize / block_size;
        assert(block_count * block_size <= kChunkSize);
        for (int i = 0; i < block_count - 1; ++i)
        {
            Block *block = (Block *)((uint8_t *)chunk->blocks + block_size * i);
            Block *next = (Block *)((uint8_t *)chunk->blocks + block_size * (i + 1));
            block->next = next;
        }
        Block *last = (Block *)((uint8_t *)chunk->blocks + block_size * (block_count - 1));
        last->next = nullptr;

        free_lists_[index] = chunk->blocks->next;
        ++num_chunk_count_;

        return chunk->blocks;
    }
}

void BlockAllocator::free(void *p, int size)
{
    if (size == 0 || p == nullptr)
    {
        return;
    }

    assert(0 < size);

    if (size > kMaxBlockSize)
    {
        ::free(p);
        return;
    }

    int index = s_block_size_lookup_[size];
    assert(0 <= index && index < kBlockSizes);

#ifdef _DEBUG
    int block_size = block_sizes_[index];
    bool found = false;
    for (int i = 0; i < num_chunk_count_; ++i)
    {
        Chunk *chunk = chunks_ + i;
        if (chunk->block_size != block_size)
        {
            assert((uint8_t *)p + block_size <= (uint8_t *)chunk->blocks ||
                (uint8_t *)chunk->blocks + kChunkSize <= (uint8_t *)p);
        }
        else
        {
            if ((uint8_t *)chunk->blocks <= (uint8_t *)p && (uint8_t *)p + block_size <= (uint8_t *)chunk->blocks + kChunkSize)
            {
                found = true;
            }
        }
    }

    assert(found);

    memset(p, 0xfd, block_size);
#endif

    Block *block = (Block *)p;
    block->next = free_lists_[index];
    free_lists_[index] = block;
}

void BlockAllocator::clear()
{
    for (int i = 0; i < num_chunk_count_; ++i)
    {
        ::free(chunks_[i].blocks);
    }

    num_chunk_count_ = 0;
    memset(chunks_, 0, num_chunk_space_ * sizeof(Chunk));
    memset(free_lists_, 0, sizeof(free_lists_));
}

														
														
														
