from pathlib import Path
from argparse import ArgumentParser
import cv2

args = ArgumentParser()
args.add_argument('img_path', metavar='image files path', type=str)
args.add_argument('vid_path', metavar='video path', type=str)
args.add_argument('fps', metavar='frames per second', type=int)
vals = args.parse_args()

direct = Path().cwd() / vals.img_path

ppms = list(filter(
    lambda file: file.suffix == '.ppm',
    direct.iterdir()
))
assert len(ppms) >= 1

ppms.sort(key = lambda ppm: int(ppm.stem.split('_')[-1]))
imgs = map(lambda ppm: cv2.imread(str(ppm)), ppms)

img_sample = cv2.imread(str(ppms[0]))
height, width, layers = img_sample.shape
fourc = cv2.VideoWriter_fourcc(*'DIVX')
video = cv2.VideoWriter(
    vals.vid_path,
    fourc,
    vals.fps,
    (width, height)
)

for img in imgs: video.write(img)
video.release()
