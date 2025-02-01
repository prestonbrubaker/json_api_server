import pygame
import requests
import time
import threading
import numpy as np

# Constants
URL = "http://50.188.120.138:5000/"
WIDTH, HEIGHT = 800, 800
AGENT_SCALE_FACTOR = 8
FOOD_GRID_SIZE = 50
FOOD_CELL_SIZE = WIDTH // FOOD_GRID_SIZE  # Each cell is 16x16 pixels
UPDATE_INTERVAL = 0.01  # 50ms
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
avg_food = 0
std_dev_food = 0
max_food = 50  # Assuming max food an agent can have is 50

def fetch_agents():
    global agents, food_grid, avg_genes, avg_food, std_dev_food, max_food
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
                
                # Compute average and standard deviation of agent food
                if agents:
                    food_values = np.array([agent["food"] for agent in agents])
                    avg_food = np.mean(food_values)
                    std_dev_food = np.std(food_values)
                    max_food = max(food_values)  # Dynamic scaling
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
            intensity = int((food / 100) * 255)  # Scale from 0 (black) to 255 (bright green)
            color = (intensity, 0, intensity / 2)
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
        
        # Ensure food values are within valid range
        if max_food > 0:
            intensity = int((food / max_food) * 255)
        else:
            intensity = 0

        # Clamp values between 0 and 255 to avoid errors
        intensity = max(0, min(255, intensity))
        
        color = (intensity, 255 - intensity, 0)  # RGB scaling

        pygame.draw.circle(screen, color, (x, y), 5)

    
    # Display average gene values
    gene_text = "Avg Genes: " + ", ".join(f"{val:.3f}" for val in avg_genes)
    gene_surface = font.render(gene_text, True, (255, 255, 255))
    screen.blit(gene_surface, (10, HEIGHT - FONT_SIZE - 40))

    # Display average food and standard deviation
    food_text = f"Avg Food: {avg_food:.2f}, Std Dev: {std_dev_food:.2f}"
    food_surface = font.render(food_text, True, (255, 255, 255))
    screen.blit(food_surface, (10, HEIGHT - FONT_SIZE - 10))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
