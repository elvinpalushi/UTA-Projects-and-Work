import sys
import argparse
import numpy
numpy.float = numpy.float64
numpy.int = numpy.int_

from PySide6 import QtCore, QtWidgets, QtGui
from PySide6.QtGui import QPainter
from skvideo.io import vread
from skimage.morphology import dilation
from skimage.measure import label, regionprops
from skimage.color import rgb2gray

class MotionDetector():
    def __init__(self, ppframe=None, pframe=None, cframe=None, alpha=10, tau=0.05, delta=150, s=1, N=5):
        self.ppframe = ppframe
        self.pframe = pframe
        self.cframe = cframe
        self.alpha = alpha
        self.tau = tau
        self.delta = delta
        self.s = s
        self.N = N
        self.frame_count = 0
        self.trackers = []
        self.history = []
        self.trail = []

    def detect_motion(self, frame, qimage):
        if self.frame_count % self.s != 0:
            return
        
        self.frame_count +=1

        threshold = self.tau

        frame = rgb2gray(frame)

        self.ppframe = self.pframe
        self.pframe = self.cframe
        self.cframe = frame

        if self.ppframe is None:
            return

        diff1 = numpy.abs(self.cframe - self.pframe)
        diff2 = numpy.abs(self.pframe - self.ppframe)

        motion_frame = numpy.minimum(diff1, diff2)
        thresh_frame = motion_frame > threshold
        dilated_frame = dilation(thresh_frame, numpy.ones((9, 9)))
        label_frame = label(dilated_frame)
        regions = regionprops(label_frame)

        painter = QPainter(qimage)
        painter.setPen(QtGui.QPen(QtCore.Qt.red, 2))

        centroids = numpy.array([region.centroid for region in regions])

        for tracker in self.trackers:
            prediction = tracker.predict()
            self.history.append(prediction)

            if len(centroids) == 0:
                tracker.inactive_frames += 1
                continue
    
            # Find closest region
            distances = numpy.linalg.norm(numpy.array([region.centroid for region in regions]) - prediction[:2], axis=1)
            closest_idx = numpy.argmin(distances)
            closest_dist = distances[closest_idx]
            closest_region = regions[closest_idx]

            # Double check that closest region is within delta
            if closest_dist < self.delta:
                tracker.update(closest_region.centroid)

                minr, minc, maxr, maxc = closest_region.bbox
                height = maxr - minr
                width = maxc - minc
                painter.drawRect(minc, minr, width, height)

                # ADDED: Keep track to draw trails at the end
                self.trail.append(closest_region.centroid)

                regions.remove(closest_region)
            else:
                tracker.inactive_frames += 1

            # If tracked object has been inactive beyond threshold, remove it
            if tracker.inactive_frames >= self.alpha:
                self.trackers.remove(tracker)

        # Remaining regions are unpaired -- add as new objects
        for region in regions:
            if len(self.trackers) >= self.N:
                break
                
            minr, minc, maxr, maxc = region.bbox
            height = maxr - minr
            width = maxc - minc
            centroid = numpy.array(region.centroid)

            painter.drawRect(minc, minr, width, height)

            self.trail.append(region.centroid)

            self.trackers.append(KalmanFilter(centroid))

        painter.setPen(QtGui.QPen(QtCore.Qt.green, 3))
        
        for motion in self.trail:
            painter.drawPoint(motion[1], motion[0])

        painter.end()

        return self.trackers


class KalmanFilter():
    def __init__(self, pos):
        super().__init__()

        self.state = numpy.zeros(4) # state matrix
        self.cov = numpy.eye(4) # covariance matrix
        self.H = numpy.eye(2, 4) # measurement function
        self.D = numpy.eye(4) # state transistion matrix
        self.D[0, 2] = 1
        self.D[1, 3] = 1
        self.R = numpy.eye(2) # measurement uncertainty/noise

        self.inactive_frames = 0

        self.state[:2] = pos

    def predict(self):
        # Predict the next state
        self.state = self.D @ self.state
        self.cov = self.D @ self.cov @ self.D.T
        
        return self.state

    def update(self, measurement):
        # Kalman filter update step
        y = measurement - self.H @ self.state
        S = self.H @ self.cov @ self.H.T + self.R
        K = self.cov @ self.H.T @ numpy.linalg.inv(S)
        self.state = self.state + K @ y
        self.cov = self.cov - K @ self.H @ self.cov

        return self.state


class QtDemo(QtWidgets.QWidget):
    def __init__(self, frames):
        super().__init__()

        self.motion_detector = MotionDetector()

        self.button = QtWidgets.QPushButton("Next Frame")
        self.skip_60_forward_button = QtWidgets.QPushButton("Skip 60 Frames Forward")
        self.skip_60_backward_button = QtWidgets.QPushButton("Skip 60 Frames Backward")
        print("Help")
        self.frames = frames
        self.current_frame = 0


        # Configure image label
        self.img_label = QtWidgets.QLabel(alignment=QtCore.Qt.AlignCenter)
        h, w, c = self.frames[0].shape
        if c == 1:
            img = QtGui.QImage(self.frames[0], w, h, QtGui.QImage.Format_Grayscale8)
        else:
            img = QtGui.QImage(self.frames[0], w, h, QtGui.QImage.Format_RGB888)
        self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))

        print("[INFO] Image shape: ", self.frames[0].shape)

        # Configure slider
        self.frame_slider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal)
        self.frame_slider.setTickInterval(1)
        self.frame_slider.setMinimum(0)
        self.frame_slider.setMaximum(self.frames.shape[0]-1)

        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.addWidget(self.img_label)
        self.layout.addWidget(self.button)
        self.layout.addWidget(self.skip_60_forward_button)
        self.layout.addWidget(self.skip_60_backward_button)
        self.layout.addWidget(self.frame_slider)

        # Connect functions
        self.button.clicked.connect(self.on_click)
        self.skip_60_forward_button.clicked.connect(self.skip_60_forward)
        self.skip_60_backward_button.clicked.connect(self.skip_60_backward)
        self.frame_slider.sliderMoved.connect(self.on_move)


    @QtCore.Slot()
    def on_click(self):
        if self.current_frame == self.frames.shape[0]-1:
            return
        h, w, c = self.frames[self.current_frame].shape
        if c == 1:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_Grayscale8)
        else:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_RGB888)

        detections = self.motion_detector.detect_motion(self.frames[self.current_frame], img)
        self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))
        self.frame_slider.setValue(self.current_frame)
        self.current_frame += 1

    # Better to press next frame 3 times before jumping 60 frames instantly
    @QtCore.Slot()
    def skip_60_forward(self):
        self.current_frame = min(self.current_frame + 60, self.frames.shape[0] - 1)

        start = max(self.current_frame - 2, 0)
        for i in range(start, self.current_frame + 1):
            h, w, c = self.frames[i].shape
            if c == 1:
                img = QtGui.QImage(self.frames[i], w, h, QtGui.QImage.Format_Grayscale8)
            else:
                img = QtGui.QImage(self.frames[i], w, h, QtGui.QImage.Format_RGB888)
            self.motion_detector.detect_motion(self.frames[i], img)

        h, w, c = self.frames[self.current_frame].shape
        if c == 1:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_Grayscale8)
        else:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_RGB888)
        self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))
        self.frame_slider.setValue(self.current_frame)


    # Uncomment this and comment the skip_60_foward function above if you want to draw boxes 
    # from the point that you are skipping from all the way to the point you are skipping to. 
    # You need to press the skip foward button around 5 times to work if you do use this.
    # @QtCore.Slot()
    # def skip_60_forward(self):
    #     start_frame = self.current_frame
    #     for frame_idx in range(start_frame, 60):
    #         h, w, c = self.frames[frame_idx].shape
    #         if c == 1:
    #             img = QtGui.QImage(self.frames[frame_idx], w, h, QtGui.QImage.Format_Grayscale8)
    #         else:
    #             img = QtGui.QImage(self.frames[frame_idx], w, h, QtGui.QImage.Format_RGB888)
    #         detections = self.motion_detector.detect_motion(self.frames[frame_idx], img)
    #         self.current_frame = frame_idx + 1
    #         print(f"At Frame {self.current_frame}")
    #     self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))
    #     self.frame_slider.setValue(self.current_frame)

    @QtCore.Slot()
    def skip_60_backward(self):
        self.current_frame -= 60
        if self.current_frame == self.frames.shape[0]-1:
            return
        h, w, c = self.frames[self.current_frame].shape
        if c == 1:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_Grayscale8)
        else:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_RGB888)

        self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))
        self.frame_slider.setValue(self.current_frame)


    @QtCore.Slot()
    def on_move(self, pos):
        self.current_frame = pos
        h, w, c = self.frames[self.current_frame].shape
        if c == 1:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_Grayscale8)
        else:
            img = QtGui.QImage(self.frames[self.current_frame], w, h, QtGui.QImage.Format_RGB888)
        
        # detections = self.motion_detector.detect_motion(self.frames[self.current_frame], img)
        self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Demo for loading video with Qt5.")
    parser.add_argument("video_path", metavar='PATH_TO_VIDEO', type=str)
    parser.add_argument("--num_frames", metavar='n', type=int, default=-1)
    parser.add_argument("--grey", metavar='True/False', type=str, default=False)
    args = parser.parse_args()

    num_frames = args.num_frames

    if num_frames > 0:
        frames = vread(args.video_path, num_frames=num_frames, as_grey=args.grey)
    else:
        frames = vread(args.video_path, as_grey=args.grey)

    print("Loaded video with shape:", frames.shape)

    app = QtWidgets.QApplication([])

    widget = QtDemo(frames)
    widget.resize(800, 600)
    widget.show()

    sys.exit(app.exec_())