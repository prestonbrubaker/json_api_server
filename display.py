import pygame
import requests
import time
import threading

# Constants
URL = "http://50.188.120.138:5000/"
WIDTH, HEIGHT = 400, 400
SCALE_FACTOR = 4
UPDATE_INTERVAL = 0.1  # 100ms

# Pygame setup
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Agent Simulation")
clock = pygame.time.Clock()

# Shared data
agents = []

def fetch_agents():
    global agents
    while True:
        try:
            response = requests.get(URL)
            if response.status_code == 200:
                agents = response.json().get("agents", [])
        except requests.RequestException:
            print("Failed to fetch agent data")
        time.sleep(UPDATE_INTERVAL)

# Start the data fetching thread
threading.Thread(target=fetch_agents, daemon=True).start()

running = True
while running:
    screen.fill((0, 0, 0))  # Clear screen with black
    
    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    # Draw agents
    for agent in agents:
        x = int(agent["x"] * SCALE_FACTOR)
        y = int(agent["y"] * SCALE_FACTOR)
        food = agent["food"]
        color = (min(255, food * 10), 255 - min(255, food * 10), 0)  # Color based on food
        pygame.draw.circle(screen, color, (x, y), 5)
    
    pygame.display.flip()
    clock.tick(60)

pygame.quit()
