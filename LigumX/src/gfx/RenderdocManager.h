#pragma once


class RenderdocManager
{
public:

	static RenderdocManager& GetInstance()
	{
		static RenderdocManager instance; // Guaranteed to be destroyed.
		return instance;
	}

	RenderdocManager() { }

	void LoadRenderdoc();

	bool GetIsEnabled() const { return m_IsEnabled; }

private:

	RenderdocManager(RenderdocManager const&) = delete;
	void operator=(RenderdocManager const&) = delete;

	bool m_IsEnabled = false;
};