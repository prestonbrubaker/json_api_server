import pygame
import requests
import time
import threading

# Constants
URL = "http://50.188.120.138:5000/"
WIDTH, HEIGHT = 400, 400
AGENT_SCALE_FACTOR = 4
FOOD_GRID_SIZE = 10
FOOD_CELL_SIZE = WIDTH // FOOD_GRID_SIZE  # Each cell is 40x40 pixels
UPDATE_INTERVAL = 0.1  # 100ms

# Pygame setup
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Agent Simulation")
clock = pygame.time.Clock()

# Shared data
agents = []
food_grid = []

def fetch_agents():
    global agents, food_grid
    while True:
        try:
            response = requests.get(URL)
            if response.status_code == 200:
                data = response.json()
                agents = data.get("agents", [])
                food_grid = data.get("food_grid", [])
        except requests.RequestException:
            print("Failed to fetch agent data")
        time.sleep(UPDATE_INTERVAL)

# Start the data fetching thread
threading.Thread(target=fetch_agents, daemon=True).start()

running = True
while running:
    screen.fill((0, 0, 0))  # Clear screen with black
    
    # Draw food grid
    for i, row in enumerate(food_grid):
        for j, food in enumerate(row):
            color = (0, min(255, food * 2), 0)  # Green intensity based on food amount
            pygame.draw.rect(screen, color, (j * FOOD_CELL_SIZE, i * FOOD_CELL_SIZE, FOOD_CELL_SIZE, FOOD_CELL_SIZE))
    
    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    # Draw agents
    for agent in agents:
        x = int(agent["x"] * AGENT_SCALE_FACTOR)
        y = int(agent["y"] * AGENT_SCALE_FACTOR)
        food = agent["food"]
        color = (min(255, food * 10), 255 - min(255, food * 10), 0)  # Color based on food
        pygame.draw.circle(screen, color, (x, y), 5)
    
    pygame.display.flip()
    clock.tick(60)

pygame.quit()
