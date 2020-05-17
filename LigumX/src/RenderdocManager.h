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

private:

	RenderdocManager(RenderdocManager const&) = delete;
	void operator=(RenderdocManager const&) = delete;

};