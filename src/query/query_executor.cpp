#include "query_executor.h"

namespace query {

void QueryExecutor::run() {
  p_shared_query_data_->printf_hv_bound_vals();
  LOG("this :%p query executor run", this);
  if (is_finished_) {
    LOG("is finished ");
    return;
  }
  if (triple_query_set_.size() == 0) {
    LOG("query_set size is 0");
    is_finished_ = true;
    return;
  }
  for (auto p_triple : triple_query_set_) {
    if (!p_triple->is_valid()) {
      is_finished_ = true;
      return;
    }
  }
  // sorted 依据备选spo数量
  std::vector<std::shared_ptr<TripleQuery>> sorted_triple_vec(
      triple_query_set_.begin(), triple_query_set_.end());
  std::sort(
      sorted_triple_vec.begin(), sorted_triple_vec.end(),
      [](std::shared_ptr<TripleQuery> p_a, std::shared_ptr<TripleQuery> p_b) {
        return p_a->cur_valid_row_bm_index_cardinality_ <
               p_b->cur_valid_row_bm_index_cardinality_;
      });
  std::set<size_t> visited_index_query_set;

  // select 策略：（1）spo数量少 (2)存在共享变量
  auto pick_next_query = [this, &sorted_triple_vec,
                          &visited_index_query_set]() -> int {
    LOG("pick next query , size :%d", sorted_triple_vec.size());
    for (size_t i = 1; i < sorted_triple_vec.size(); i++) {
      LOG("pick next query , index :%d", i);
      if (visited_index_query_set.find(i) != visited_index_query_set.end()) {
        continue;
      }
      LOG("pick next query , p_shared :%p", p_shared_query_data_.get());
      for (auto& var_name : sorted_triple_vec[i]->var_vec_) {
        LOG("pick next query : %s", var_name.c_str());
        if (p_shared_query_data_->contains_var_in_bounds_vals_map(var_name)) {
          LOG("pick next query , find : %d", i);
          return i;
        }
      }
    }
    return -1;
  };

  auto p_triple_query = sorted_triple_vec[0];
  LOG("query executor select 0");
  p_triple_query->select();
  LOG("======================");
  p_shared_query_data_->printf_hv_bound_vals();
  LOG("======================");
  if (!p_triple_query->is_valid()) {
    is_empty_result_ = true;
    return;
  }
  LOG("query executor select 0 finish");
  visited_index_query_set.insert(0);
  LOG("go on selec");
  LOG("======================");
  p_shared_query_data_->printf_hv_bound_vals();
  LOG("======================");
  do {
    int query_index = pick_next_query();
    LOG("select next query : %d", query_index);
    if (-1 == query_index) {
      return;
    }
    sorted_triple_vec[query_index]->select();
    // TODO :这个地方得改进
    // is_valid() 为false就应该提前终止query,因为有一个spo query没有结果
    if (!sorted_triple_vec[query_index]->is_valid()) {
      is_empty_result_ = true;
      return;
    }
    visited_index_query_set.insert(query_index);
  } while (true);
  LOG("query executor run finished ");
}

int QueryExecutor::get_min_growth_tree_by_kruskal() {
  std::vector<std::shared_ptr<TripleQuery>> sorted_triple_vec;
  for (auto p_triple_query : triple_query_set_) {
    if (p_triple_query->var_vec_.size() < 2) {
      continue;
    }
    sorted_triple_vec.push_back(p_triple_query);
  }
  // sorted
  std::sort(
      sorted_triple_vec.begin(), sorted_triple_vec.end(),
      [](std::shared_ptr<TripleQuery> p_a, std::shared_ptr<TripleQuery> p_b) {
        return p_a->cur_valid_row_bm_index_cardinality_ <
               p_b->cur_valid_row_bm_index_cardinality_;
      });
  std::set<std::string> visited_var_node;
  for (auto p_triple_query : sorted_triple_vec) {
    size_t find_count = 0;
    for (auto& var_name : p_triple_query->var_vec_) {
      if (visited_var_node.find(var_name) != visited_var_node.end()) {
        find_count++;
        visited_var_node.insert(var_name);  // trick
      }
    }
    if (find_count < 2) {
      min_growth_tree_.push_back(p_triple_query);
    }
  }
  return 0;
}

int QueryExecutor::shrink_min_growth_tree() {
  std::unordered_map<std::string, size_t> node_degree;
  for (auto p_triple_query : min_growth_tree_) {
    for (auto& var_name : p_triple_query->var_vec_) {
      node_degree[var_name]++;
    }
  }

  std::set<size_t> visited_triple_query;
  std::string last_merged_var_name;
  auto shrink_step = [this, &node_degree, &visited_triple_query,
                      &last_merged_var_name]() -> int {
    for (size_t i = 0; i < this->min_growth_tree_.size(); i++) {
      if (visited_triple_query.find(i) != visited_triple_query.end()) {
        continue;
      }
      auto p_triple_query = this->min_growth_tree_[i];
      for (auto& var_name : p_triple_query->var_vec_) {
        if (node_degree[var_name] == 1) {
          std::string merged_var_name =
              p_triple_query->update_cartesian_product_in_shared_data(var_name);
          node_degree[var_name]--;
          node_degree[merged_var_name]--;
          visited_triple_query.insert(i);
          LOG("merge var name : %s", merged_var_name.c_str());
          last_merged_var_name = merged_var_name;
          return 0;
        }
      }
    }
    return -1;
  };
  while (visited_triple_query.size() != this->min_growth_tree_.size()) {
    if (-1 == shrink_step()) {
      fprintf(stderr, "shrink_step error :-1\n");
      return -1;
    }
  }
  last_merged_var_name_ = last_merged_var_name;
  is_finished_ = true;
  return 0;
}

void QueryExecutor::make_result() {
  size_t result_col_num_ = p_shared_query_data_->var_val_type_.size();
  auto make_empty_result = [this, result_col_num_]() {
    std::string null_result;
    for (size_t i = 0; i < result_col_num_; i++) {
      null_result.append("null\t");
    }
    this->result_matrix_.push_back(null_result);
  };

  LOG("will make result");
  if (is_empty_result_) {
    make_empty_result();
    return;
  }
  LOG("will get min_growth tree by kruska");
  if (-1 == get_min_growth_tree_by_kruskal()) {
    make_empty_result();
    return;
  }

  LOG("will shrink min_growth tree by kruska");
  if (-1 == shrink_min_growth_tree()) {
    make_empty_result();
    return;
  }
  // for debug
  p_shared_query_data_->printf_intermediate_result();
  p_shared_query_data_->printf_intermediate_result_col_name();

  // create col_name
  auto got_merged_node_col_name =
      p_shared_query_data_->intermediate_result_col_name_.find(
          last_merged_var_name_);
  if (got_merged_node_col_name ==
      p_shared_query_data_->intermediate_result_col_name_.end()) {
    fprintf(stderr, "merged_tree_map node name not found!\n");
    exit(-1);
  }

  result_col_name_.push_back(last_merged_var_name_);
  for (auto& col_name : got_merged_node_col_name->second) {
    result_col_name_.push_back(col_name);
  }

  // create col_val
  auto got_merged_tree_map =
      p_shared_query_data_->intermediate_result_.find(last_merged_var_name_);
  if (got_merged_tree_map == p_shared_query_data_->intermediate_result_.end()) {
    fprintf(stderr, "merged_tree_map node not found!\n");
    exit(-1);
  }
  auto merged_tree_map = got_merged_tree_map->second;
  for (auto val_iter = merged_tree_map.begin();
       val_iter != merged_tree_map.end(); val_iter++) {
    std::string val_key = val_iter->first;
    std::vector<std::string> val_vec = val_iter->second;
    for (auto& val_value : val_vec) {
      std::string line_value(val_key);
      line_value.append("\t").append(val_value);
      result_matrix_.push_back(line_value);
    }
  }
}

const std::string QueryExecutor::format_query_result() const {
  std::unordered_map<std::string, size_t> result_matrix_pos;
  for (size_t i = 0; i < result_col_name_.size(); i++) {
    result_matrix_pos[result_col_name_[i]] = i;
  }

  std::vector<int> aim_pos;  // pos -1 means undefined var,will output 'null'
  for (auto& col_name : p_shared_query_data_->need_output_vars_) {
    auto got_pos = result_matrix_pos.find(col_name);
    if (got_pos == result_matrix_pos.end()) {
      aim_pos.push_back(-1);
    } else {
      aim_pos.push_back(got_pos->second);
    }
  }

  std::stringstream ss;
  ss << "====================query result======================\n";
  for (auto& col_name : p_shared_query_data_->need_output_vars_) {
    ss << col_name << "\t";
  }
  ss << "\n";
  for (auto& row_value : result_matrix_) {
    std::vector<std::string> splited_row_value;
    std::string elem;
    for (const char& c : row_value) {
      if (c == '\t') {
        splited_row_value.push_back(elem);
        elem.clear();
        continue;
      }
      elem.push_back(c);
    }
    splited_row_value.push_back(elem);

    for (auto& pos : aim_pos) {
      if (pos == -1) {
        ss << "null";
      } else {
        ss << splited_row_value[pos];
      }
      ss << "\t";
    }
    ss << "\n";
  }
  ss << "======================================================\n";
  return ss.str();
}

}  // namespace query
