#include "pch.h"
#include "Player.h"

Player::Player(PlayerType type) : isHandHidden(false), playerType(type)
{

}

void Player::RemoveSelectedCard()
{
	if (selectedCardIndex < 0 || selectedCardIndex >= hand.size()) {
		throw IndexOutOfRangeException("Selected card index is out of range.");
	}

	Card cardToRemove = GetHand()[selectedCardIndex];
	auto it = std::find(hand.begin(), hand.end(), cardToRemove);
	if (it != hand.end()) {
		hand.erase(it);
	}
	selectedCardIndex = -1;
}

#pragma region Getters and Setters
void Player::SetHand(std::vector<Card>& newHand)
{
	hand = newHand;
}

std::vector<Card> Player::GetHand() const
{
	return hand;
}

void Player::SetPlayerName(const CString& name)
{
	playerName = name;
}

CString Player::GetPlayerName() const
{
	return playerName;
}

void Player::SetSelectCardIndex(int index)
{
	selectedCardIndex = index;
}

int Player::GetSelectCardIndex() const
{
	return selectedCardIndex;
}
#pragma endregion

