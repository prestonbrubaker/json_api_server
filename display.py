import pygame
import requests
import time
import threading
import numpy as np

# Constants
URL = "http://50.188.120.138:5000/"
WIDTH, HEIGHT = 800, 800
AGENT_SCALE_FACTOR = 8
FOOD_GRID_SIZE = 10
FOOD_CELL_SIZE = WIDTH // FOOD_GRID_SIZE  # Each cell is 40x40 pixels
UPDATE_INTERVAL = 0.05  # 50ms
FONT_SIZE = 30

# Pygame setup
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Agent Simulation")
clock = pygame.time.Clock()
font = pygame.font.Font(None, FONT_SIZE)

# Shared data
agents = []
food_grid = []
avg_genes = [0] * 10

def fetch_agents():
    global agents, food_grid, avg_genes
    while True:
        try:
            response = requests.get(URL)
            if response.status_code == 200:
                data = response.json()
                agents = data.get("agents", [])
                food_grid = data.get("food_grid", [])
                
                # Compute average gene values
                if agents:
                    gene_arrays = np.array([agent["genes"] for agent in agents])
                    avg_genes = np.mean(gene_arrays, axis=0).tolist()
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
            pygame.draw.rect(screen, color, (i * FOOD_CELL_SIZE, j * FOOD_CELL_SIZE, FOOD_CELL_SIZE, FOOD_CELL_SIZE))
    
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
    
    # Display average gene values
    gene_text = "Avg Genes: " + ", ".join(f"{val:.2f}" for val in avg_genes)
    text_surface = font.render(gene_text, True, (255, 255, 255))
    screen.blit(text_surface, (10, HEIGHT - FONT_SIZE - 10))
    
    pygame.display.flip()
    clock.tick(60)

pygame.quit()