import numpy as np
import pygame as pg

class SoftwareRender:
    def __init__(self) -> None:
        pg.init()
        self.RES = self.WIDTH, self.HEIGHT = 1920, 1080
        self.FPS = 60
        self.screen = pg.display.set_mode(self.RES)
        self.clock = pg.time.Clock()

    def draw(self) -> None:
        self.screen.fill(pg.Color('darkslategray'))

    def run(self):
        while True:
            self.draw()
            for i in pg.event.get():
                if i.type == pg.QUIT:
                    exit()
            pg.display.set_caption(str(self.clock.get_fps()))
            pg.display.flip()
            self.clock.tick(self.FPS)

if __name__ == '__main__':
    app = SoftwareRender()
    app.run()
