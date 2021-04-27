//
// Created by LiuLiangFu on 2021/4/20.
//

#include "Board.h"
#include <sstream>
#include <iostream>
#include <iomanip>

std::vector<unsigned short> get_indexes(const std::vector<bool> &vec) {
    std::vector<unsigned short> ret;
    for (unsigned short i = 0; (unsigned short) i < vec.size(); i++) {
        if (vec[i]) {
            ret.push_back(i);
        }
    }
    return ret;
}

Board::Board(unsigned short x, unsigned short y, unsigned short n_shapes, short immovable_shape, short space_shape,
             bool immovable_interactive)
        : x_(x), y_(y), n_shapes_(n_shapes),
          immovable_shape_(immovable_shape),
          space_shape_(space_shape),
          immovable_interactive_(immovable_interactive) {
    size_ = x_ * y_;
    data_.clear();
    data_.reserve(size_);
    for (unsigned short index = 0; index < size_; index++) {
        data_.push_back(space_shape_);
    }
}

std::string Board::to_string() const {
    std::stringstream ss;
    for (unsigned short y = 0; y < y_; y++) {
        //ss << std::setw(4) << y * x_ << "|";
        for (unsigned short x = 0; x < x_; x++) {
            ss << std::setw(3) << data_[y * x_ + x];
        }
        ss << std::endl;
    }
    ss << std::endl;
    return ss.str();
}

std::vector<unsigned short> Board::shape() const {
    return {y_, x_};
}

unsigned short Board::size() const {
    return size_;
}

std::vector<short> Board::observation() const {
    return data_;
}

short Board::immovable_shape() const {
    return immovable_shape_;
}

short Board::space_shape() const {
    return space_shape_;
}

unsigned short Board::n_shapes() const {
    return n_shapes_;
}

std::vector<bool> Board::operator==(short shape) {
    std::vector<bool> ret(x_ * y_, false);
    for (unsigned short index = 0; index < size_; index++) {
        ret[index] = (data_[index] == shape);
    }
    return ret;
}

std::vector<bool> Board::operator!=(short shape) {
    std::vector<bool> ret(x_ * y_, false);
    for (unsigned short index = 0; index < size_; index++) {
        ret[index] = (data_[index] != shape);
    }
    return ret;
}

bool Board::put(const std::vector<unsigned short> &indexes, const std::vector<short> &shapes) {
    if (indexes.size() != shapes.size())
        return false;
    for (unsigned short i = 0; (unsigned short) i < indexes.size(); i++) {
        data_[indexes[i]] = shapes[i];
    }
}

void Board::delete_match(const std::vector<unsigned short> &indexes) {
    for (unsigned short index:indexes) {
        data_[index] = space_shape();
    }
}

bool Board::apply_action(unsigned int action) {
    unsigned int up_down = x_ * (y_ - 1);
    unsigned int left_right = y_ * (x_ - 1);
    if (action < up_down) {
        if ((data_[action] == immovable_shape_ || data_[action + x_] == immovable_shape_) && !immovable_interactive_)
            return false;
        std::swap(data_[action], data_[action + x_]);
        return true;
    } else if (action < up_down + left_right) {
        unsigned int resume = action - up_down;
        unsigned int index = (resume / (x_ - 1)) * y_ + (resume % (x_ - 1));
        if ((data_[index] == immovable_shape_ || data_[index + 1] == immovable_shape_) && !immovable_interactive_)
            return false;
        std::swap(data_[index], data_[index + 1]);
        return true;
    }
    return false;
}

unsigned int Board::action_space() const {
    return x_ * (y_ - 1) + y_ * (x_ - 1);
}

void Board::clear() {
    data_.clear();
    data_.reserve(size_);
    for (unsigned short index = 0; index < size_; index++) {
        data_.push_back(space_shape_);
    }
}
