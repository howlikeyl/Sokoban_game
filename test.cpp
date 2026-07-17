// Copyright 2025 Senghak

#define BOOST_TEST_MODULE SokobanPS4b
#include <boost/test/included/unit_test.hpp>
#include "Sokoban.hpp"

namespace SBT = SB;

static void writeLevelFile(const std::string& filename, const std::string& body) {
  std::ofstream out(filename);
  out << body;
  out.close();
  BOOST_REQUIRE_MESSAGE(out.good(), "Failed to write level file: " << filename);
}

// 1) Push → Win → Reset
BOOST_AUTO_TEST_CASE(push_box_and_win_then_reset) {
  const std::string L =
    "3 5\n"
    "#####\n"
    "#@A.a\n"
    "#####\n";
  writeLevelFile("level_push_to_win.lvl", L);
  SBT::Sokoban g("level_push_to_win.lvl");

  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);
  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);
  BOOST_CHECK(g.isWon());

  g.reset();
  BOOST_CHECK(!g.isWon());
}

// 2) Box against wall blocks
BOOST_AUTO_TEST_CASE(box_against_wall_blocks) {
  const std::string L =
    "3 6\n"
    "######\n"
    "#@A##.\n"
    "######\n";
  writeLevelFile("level_box_wall_block.lvl", L);
  SBT::Sokoban g("level_box_wall_block.lvl");

  auto p0 = g.playerLoc();
  g.movePlayer(SBT::Direction::Right);
  auto p1 = g.playerLoc();
  BOOST_CHECK_EQUAL(p1.x, p0.x);
  BOOST_CHECK_EQUAL(p1.y, p0.y);
}

// 3) Cannot push two boxes at once
BOOST_AUTO_TEST_CASE(cannot_push_two_boxes_safe) {
  const std::string L =
    "3 8\n"
    "########\n"
    "#@AA.a##\n"
    "########\n";
  writeLevelFile("level_two_boxes_safe.lvl", L);
  SBT::Sokoban g("level_two_boxes_safe.lvl");

  auto p0 = g.playerLoc();
  g.movePlayer(SBT::Direction::Right);
  auto p1 = g.playerLoc();
  BOOST_CHECK_EQUAL(p1.x, p0.x);
  BOOST_CHECK_EQUAL(p1.y, p0.y);
}

// 4) Player cannot move off-screen
BOOST_AUTO_TEST_CASE(player_cannot_move_off_screen) {
  const std::string L =
    "3 3\n"
    "###\n"
    "#@#\n"
    "###\n";
  writeLevelFile("level_player_edge.lvl", L);
  SBT::Sokoban g("level_player_edge.lvl");

  auto p0 = g.playerLoc();

  // Try to move left into wall
  g.movePlayer(SBT::Direction::Left);
  auto p1 = g.playerLoc();
  BOOST_CHECK_EQUAL(p1.x, p0.x);
  BOOST_CHECK_EQUAL(p1.y, p0.y);

  // Try to move up off-screen
  g.movePlayer(SBT::Direction::Up);
  auto p2 = g.playerLoc();
  BOOST_CHECK_EQUAL(p2.x, p0.x);
  BOOST_CHECK_EQUAL(p2.y, p0.y);

  // Try to move down into wall
  g.movePlayer(SBT::Direction::Down);
  auto p3 = g.playerLoc();
  BOOST_CHECK_EQUAL(p3.x, p0.x);
  BOOST_CHECK_EQUAL(p3.y, p0.y);

  // Try to move right into wall
  g.movePlayer(SBT::Direction::Right);
  auto p4 = g.playerLoc();
  BOOST_CHECK_EQUAL(p4.x, p0.x);
  BOOST_CHECK_EQUAL(p4.y, p0.y);
}

// 5) Box cannot be pushed off-screen
BOOST_AUTO_TEST_CASE(box_cannot_be_pushed_off_screen) {
  const std::string L =
    "3 4\n"
    "####\n"
    "#@A#\n"
    "####\n";
  writeLevelFile("level_box_edge.lvl", L);
  SBT::Sokoban g("level_box_edge.lvl");

  auto p0 = g.playerLoc();
  g.movePlayer(SBT::Direction::Right);
  auto p1 = g.playerLoc();
  BOOST_CHECK_EQUAL(p1.x, p0.x);
  BOOST_CHECK_EQUAL(p1.y, p0.y);
}

// 6) Win with many boxes ≥ targets
BOOST_AUTO_TEST_CASE(win_many_boxes_cover_all_targets) {
  const std::string L =
    "3 7\n"
    "#######\n"
    "#@A.aA#\n"
    "#######\n";
  writeLevelFile("level_many_boxes_one_target.lvl", L);
  SBT::Sokoban g("level_many_boxes_one_target.lvl");

  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);
  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);
  BOOST_CHECK(g.isWon());
}

// 7) Win when all boxes are on targets
BOOST_AUTO_TEST_CASE(win_many_targets_all_boxes_on_targets) {
  const std::string L =
    "3 8\n"
    "########\n"
    "#@A.a.a#\n"
    "########\n";
  writeLevelFile("level_many_targets_one_box.lvl", L);
  SBT::Sokoban g("level_many_targets_one_box.lvl");

  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);
  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);
  BOOST_CHECK(g.isWon());
}

// 8) Unknown symbol is NOT floor
BOOST_AUTO_TEST_CASE(missing_symbol_guard_wall_not_floor) {
  const std::string L =
    "3 6\n"
    "######\n"
    "#@% .#\n"
    "######\n";
  writeLevelFile("level_unknown_symbol.lvl", L);
  SBT::Sokoban g("level_unknown_symbol.lvl");

  auto p0 = g.playerLoc();
  g.movePlayer(SBT::Direction::Right);
  auto p1 = g.playerLoc();
  BOOST_CHECK_EQUAL(p1.x, p0.x);
  BOOST_CHECK_EQUAL(p1.y, p0.y);
}

// 9) Autowin at start
BOOST_AUTO_TEST_CASE(autowin_at_start) {
  const std::string L =
    "3 5\n"
    "#####\n"
    "#1a.#\n"
    "#####\n";
  writeLevelFile("level_autowin_start.lvl", L);
  SBT::Sokoban g("level_autowin_start.lvl");

  BOOST_CHECK(g.isWon());
}

// 10) Swap test: box moved off target
BOOST_AUTO_TEST_CASE(swap_box_off_target_not_won) {
  const std::string L =
    "3 7\n"
    "#######\n"
    "#@aA .#\n"
    "#######\n";
  writeLevelFile("level_swap_off.lvl", L);
  SBT::Sokoban g("level_swap_off.lvl");

  BOOST_CHECK(!g.isWon());
  g.movePlayer(SBT::Direction::Right);  // move onto target
  g.movePlayer(SBT::Direction::Right);  // push box off target
  BOOST_CHECK(!g.isWon());              // should not be won anymore
}
