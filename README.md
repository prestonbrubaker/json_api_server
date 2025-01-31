# C JSON Server

This is a simple C-based HTTP server that serves dynamically generated JSON data. It will be used to centralize the compute for agent in an evolution simulation.

## Features
- Uses `libmicrohttpd` to handle HTTP requests
- Generates random "agents" with positions, food levels, and genetic data
- Outputs structured JSON for easy integration with clients for purposes such as visualization
- Runs on Ubuntu with minimal dependencies

## Installation

First, install `libmicrohttpd`:

```sh
sudo apt update
sudo apt install libmicrohttpd-dev
