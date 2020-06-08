#!/usr/bin/env python

"""Tests for `dodger` package."""

import pytest


from dodger import Dodger


@pytest.fixture
def dodger():
    """Sample pytest fixture.

    See more at: http://doc.pytest.org/en/latest/fixture.html
    """
    # import requests
    # return requests.get('https://github.com/audreyr/cookiecutter-pypackage')
    with Dodger() as dodger:
        return dodger


def test_content(dodger):
    """Sample pytest test function with the pytest fixture as an argument."""
    dodger.dodge()