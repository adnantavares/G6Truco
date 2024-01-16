#include "pch.h"
#include "Round.h"

Round::Round()
{

}

Round::Round(std::array<Player*, 4>& players) {
    for (auto& player : players) {
        player->SetRaiseBetCallback(std::bind(&Round::OnRaiseBet, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void Round::OnRaiseBet(Player* player, int bet) {
    
}