#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
  /// TODO: Implement this
  return std::holds_alternative<sql::state::Valid>(state_);
}

void SqlValidator::handle(Token token) {
  /// TODO: Implement this
  int ind = state_.index();
  if (ind == 0){
      auto new_state = state::Start{};
      state_ = transition(new_state, token);  
  } else if (ind == 1){
      auto new_state = state::Invalid{};
      state_ = transition(new_state, token);  
  } else if (ind == 2){
      auto new_state = state::Valid{};
      state_ = transition(new_state, token);  
  } else if (ind == 3) { 
      auto new_state = state::SelectStmt{};
      state_ = transition(new_state, token);  
  } else if (ind == 4) {
      auto new_state = state::AllColumns{};
      state_ = transition(new_state, token);  
  } else if (ind == 5) {
      auto new_state = state::NamedColumn{};
      state_ = transition(new_state, token);  
  } else if (ind == 6) {
      auto new_state = state::MoreColumns{};
      state_ = transition(new_state, token);  
  } else if (ind == 7) {
      auto new_state = state::FromClause{};
      state_ = transition(new_state, token);  
  } else {
      auto new_state = state::TableName{};
      state_ = transition(new_state, token);  
  }}

bool is_valid_sql_query(std::vector<Token> tokens) {
  sql::SqlValidator validator{};
  for (auto& token:tokens) {
    validator.handle(token);
  }
  return validator.is_valid();
}


struct TransitionFromStartVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Select) const { return state::SelectStmt{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

struct TransitionFromValidVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Semicolon) const { return state::Valid{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

struct TransitionFromSelectStmtVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Asterisks) const { return state::AllColumns{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromAllColumnsVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::From) const { return state::FromClause{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

struct TransitionFromNamedColumnVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Comma) const { return state::MoreColumns{}; }
  State operator()(token::From) const { return state::FromClause{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Identifier) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromMoreColumnsVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Identifier) const { return state::NamedColumn{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromFromClauseVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Identifier) const { return state::TableName{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromTableNameVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Semicolon) const { return state::Valid{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};


State transition(state::Start, Token token) {
  return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::Valid, Token token) {
  return std::visit(TransitionFromValidVisitor{}, token.value());
}

State transition(state::Invalid, Token token) {
  return state::Invalid{};
}

State transition(state::SelectStmt, Token token) {
  return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}
  
State transition(state::AllColumns, Token token) {
  return std::visit(TransitionFromAllColumnsVisitor{}, token.value());
}

State transition(state::NamedColumn, Token token) {
  return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}

State transition(state::MoreColumns, Token token) {
  return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}

State transition(state::FromClause, Token token) {
  return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}

State transition(state::TableName, Token token) {
  return std::visit(TransitionFromTableNameVisitor{}, token.value());
}
} // namespace sql
