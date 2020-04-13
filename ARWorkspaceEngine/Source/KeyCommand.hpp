#pragma once

#include <optional>

class ARVirtualScreen;
namespace ARWorkspace {

class KeyCommand
{
public:
	KeyCommand() = delete;
	KeyCommand(std::shared_ptr<ARVirtualScreen> arg_p_model);
public:
	void Update();
private:
	std::optional<std::tuple<int, int>> getPositionOffset() const;
private:

	std::shared_ptr<ARVirtualScreen>	p_model;

	// ‘€ìƒKƒCƒh•\¦‚ÌŒŸ“¢.
};

}

