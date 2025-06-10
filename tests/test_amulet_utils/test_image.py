from unittest import TestCase
import os

from PIL import Image

from amulet.utils.image import (
    get_missing_no_icon,
    missing_no_icon_path,
)


class ImageTestCase(TestCase):
    def test_get_image(self) -> None:
        missing_no_icon = get_missing_no_icon()
        try:
            self.assertIsInstance(missing_no_icon, Image.Image)
        finally:
            missing_no_icon.close()

    def test_image_paths(self) -> None:
        self.assertIsInstance(missing_no_icon_path, str)
        self.assertTrue(os.path.isfile(missing_no_icon_path))
