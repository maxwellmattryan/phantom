/*
  ==============================================================================

    IComponent.h
    Created: 05 May 2021 22:08:46
    Author:  Matthew Maxwell

  ==============================================================================
*/

#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "JuceHeader.h"

#include "../editor/PhantomLookAndFeel.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

/**
 * This class contains the methods and common attributes of component classes, which hold 
 * things like visual elements or parameter sliders.
 */
class IComponent : public Component
{
public:
    IComponent(PhantomLookAndFeel& plf, AudioProcessorValueTreeState& vts) : m_lookAndFeel(plf), m_parameters(vts)
    {
        setLookAndFeel(&plf);
    };
    
    ~IComponent()
    {
        setLookAndFeel(nullptr);
    };

    /**
     * For initializing the component (i.e. configuring sliders, clearing data)
     */
    virtual void init() = 0;

    /**
     * For resetting the component sliders (useful for initializing patches).
     */
    virtual void reset() { };

    /**
     * For updating the component's visual layout, usually called by its parent.
     * @param bounds The `Rectangle` object to use in sectioning this component.
     */
    void update(Rectangle<int>& bounds)
    {
        setBounds(bounds);
    };

    /**
     * For updating the component's visual layout, usually called by its parent.
     * @param margin The margin to use in spacing the inner components of this component.
     * @param sliderDiameter The diameter (px) to use for an individual slider.
     * @param bounds The `Rectangle` object to use in sectioning this component.
     */
    void update(const int margin, Rectangle<int>& bounds)
    {
        m_margin = margin;

        update(bounds);
    };
    
    /**
     * For updating the component's visual layout, usually called by its parent.
     * @param margin The margin to use in spacing the inner components of this component.
     * @param sliderDiameter The diameter (px) to use for an individual slider.
     * @param bounds The `Rectangle` object to use in sectioning this component.
     */
    void update(const int margin, const int sliderDiameter, Rectangle<int>& bounds)
    {
        m_sliderDiameter = sliderDiameter;

        update(margin, bounds);
    };

    /**
     * For updating the component's visual layout, usually called by its parent.
     * @param margin The margin to use in spacing the inner components of this component.
     * @param sliderDiameter The diameter (px) to use for an individual slider.
     * @param windowWidth The value to use for the global window width.
     * @param windowWidth The value to use for the global window height.
     * @param bounds The `Rectangle` object to use in sectioning this component.
     */
    void update(const int margin, const int sliderDiameter, const int windowWidth, const int windowHeight, Rectangle<int>& bounds)
    {
        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;

        update(margin, sliderDiameter, bounds);
    };

    /**
     * For updating the component's visual layout, usually called by its parent.
     * @param margin The margin to use in spacing the inner components of this component.
     * @param sliderDiameter The diameter (px) to use for an individual slider.
     * @param window The `Rectangle` object to use in calculating window dimensions.
     * @param bounds The `Rectangle` object to use in sectioning this component.
     */
    void update(const int margin, const int sliderDiameter, Rectangle<int>& window, Rectangle<int>& bounds)
    {
        update(margin, sliderDiameter, window.getWidth(), window.getHeight(), bounds);
    };

    /**
     * Computes a slightly larger slider diameter.
     * @returns The diameter (px) of a larger slider.
     */
    int getLargeSliderDiameter()
    {
        return m_windowWidth * PhantomLookAndFeel::_LARGE_SLIDER_PER_WIDTH;
    };

    /**
     * Determines if a slider is larger or not.
     * @returns true if the slider diameter is equal to a large slider diameter.
     */
    bool isLargeSlider(const int sliderDiameter)
    {
        return sliderDiameter == getLargeSliderDiameter();
    }

    /**
     * Prepares a Rectangle for a slider so that it draws correctly.
     * @param removeOtherHalf Boolean value that if true will remove the left half of the given Rectangle area.
     * @param expandBy An interger value to expand the bottom of the Rectangle area by.
     * @param bounds The Rectangle object itself to use as bounds for a component.
     */
    void prepareForSlider(const bool removeOtherHalf, const int expandBy, Rectangle <int>& bounds)
    {
        prepareForSlider(m_sliderDiameter, removeOtherHalf, expandBy, bounds);
    }

    /**
     * Prepares a Rectangle for a slider so that it draws correctly.
     * @param sliderDiameter The slider diameter to use in sectioning space for the slider.
     * @param removeOtherHalf Boolean value that if true will remove the left half of the given Rectangle area.
     * @param expandBy An interger value to expand the bottom of the Rectangle area by.
     * @param bounds The Rectangle object itself to use as bounds for a component.
     */
    void prepareForSlider(const int sliderDiameter, const bool removeOtherHalf, const int expandBy, Rectangle<int>& bounds)
    {
        if(removeOtherHalf)
            bounds.removeFromLeft(bounds.getWidth() / 2.0f);

        const int padding = (bounds.getWidth() - sliderDiameter) * 0.25f;
        const bool isLarge = isLargeSlider(sliderDiameter);

        bounds.removeFromTop(isLarge ? padding * 1.5f : padding * 2.0f);
        bounds.removeFromRight(padding);
        bounds.removeFromBottom(isLarge ? padding * 0.5f : padding * 0.0f);
        bounds.removeFromLeft(padding);

        if(expandBy > 0)
        {
            bounds.expand(0, expandBy);

            bounds.removeFromTop(expandBy);
        }
    }

    /**
     * Removes the margin space around a given Rectangle.
     * @param rectangle The Rectangle to remove the margin space around.
     */
    void removeMarginAround(Rectangle <int>& rectangle)
    {
        removeMarginAround(m_margin, rectangle);
    }

    /**
     * Removes the margin space around a given Rectangle.
     * @param margin The amount of space to remove on each side.
     * @param rectangle The Rectangle to remove the margin space around.
     */
    void removeMarginAround(const int margin, Rectangle<int>& rectangle)
    {
        rectangle.removeFromTop(margin);
        rectangle.removeFromRight(margin);
        rectangle.removeFromBottom(margin);
        rectangle.removeFromLeft(margin);
    }

    /**
     * Removes space from the top of a component for the purpose of making room for the title (in the background PNG).
     * @param rectangle The Rectangle to remove the top space from.
     */
    void removeTitleSpaceFrom(Rectangle<int>& rectangle)
    {
        removeTitleSpaceFrom(m_margin, rectangle);
    }

    /**
     * Removes space from the top of a component for the purpose of making room for the title (in the background PNG).
     * @param margin The amount of space to remove from the top.
     * @param rectangle The Rectangle to remove the top space from.
     */
    void removeTitleSpaceFrom(const int margin, Rectangle<int>& rectangle)
    {
        rectangle.removeFromTop(margin);
    }

    /**
     * For referencing the look and feel from places that can't access the member variable, such as `PopupMenu`s in 
     * button lambdas.
     * @returns A reference to the look and feel member variable.
     */
    PhantomLookAndFeel& getLookAndFeel()
    {
        return m_lookAndFeel;
    }

    /**
     * For retrieving the margin (px) member variable from this component.
     * @returns The calculated margin member variable of this component.
     */
    int getMargin()
    {
        return m_margin;
    }

    /**
     * NOTE: Each individual component must override and define its own `paint(Graphics& g)` and `resized()`
     * methods in order to be drawn to the screen.
     */

    /**
     * CAUTION: For each parameter with a slider attachment, you MUST
     * delete the attachment before deleting the slider.
     */

protected:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IComponent)

    /** A reference to the look and feel object, managed by the editor. */
    PhantomLookAndFeel& m_lookAndFeel;

    /** A reference to the plugin state. */
    AudioProcessorValueTreeState& m_parameters;

    /** The width value for general text boxes. */
    int m_textBoxWidth;

    /** The height value for general text boxes. */
    int m_textBoxHeight;

    /**
     * CAUTION: The 4 variables' initializations below are specific to the resolution set in the editor,
     * which is 720p or 1280x720.
     */ 

    /** The width of the plugin window. */
    int m_windowWidth = 1280;

    /** The height of the plugin window. */
    int m_windowHeight = 720;

    /** The margin to use in spacing the inner components of this component. */
    int m_margin = 1280 * (7 / 238);

    /** The slider diameter to use in spacing the sliders of this component. */
    int m_sliderDiameter = 1280 * (1 / 20);
};

#endif
